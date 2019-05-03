#include "logger.h"

Logger::Logger() 
{
    std::experimental::filesystem::create_directories(logfileName_);

    std::fstream curentfilenumber(txlogfilenumber_, std::fstream::in | std::fstream::out | std::fstream::trunc);
    if (curentfilenumber.is_open())
    {
        if (curentfilenumber.peek() == std::ifstream::traits_type::eof())
            curentfilenumber << "0";

        curentfilenumber >> currentFile_;
        if (currentFile_ >maxfile_)
            currentFile_ = 0;
    }

    threadHandler_ = std::make_unique<std::thread>(&Logger::Writing, this);

    outStreamFile_.open(txlogfilename_, std::ios::out | std::ios::app); 
    plotStreamFile_.open(txplotfilename_, std::ios::out); 
}

Logger &Logger::SetError(Severity error, std::string file, int line)
{
    std::lock_guard<std::mutex> lock(mutex_);
    severity_ = error;
    line_ = line;
    file_ = file;
    statistics[error]++;
    return *this;
}

int Logger::GetStatistics(Severity severity)
{
    std::lock_guard<std::mutex> lock(mutex_);
    return statistics[severity];
}

void Logger::ChangeFile()
{
    UpdateCurrentFileNumber();

    outStreamFile_.close();
    std::stringstream ss;
    ss << txlogfilename_ << "." << currentFile_;
    rename(txlogfilename_.c_str(), ss.str().c_str());

    outStreamFile_.open(txlogfilename_, std::ofstream::out);
}

void Logger::UpdateCurrentFileNumber()
{
    currentFile_++;
    if (currentFile_ > maxfile_)
        currentFile_ = 0;
    std::fstream curentfilenumber(txlogfilenumber_, std::fstream::in | std::fstream::out | std::fstream::trunc);
    curentfilenumber << currentFile_;
}

void Logger::Writing()
{
    while (continueLogging_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(looptimeout_));

        std::string tmpOut;
        int writtenLine = 0;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (loggingQueue_.empty())
                continue;
            for (int t = 0; t < lineforloop_; t++)
            {
                auto line = loggingQueue_.front();
                writtenLine++;
                
                if(std::get<0>(line)==Severity::plot)
                {
                    plotStreamFile_ << std::get<1>(line) << std::flush;
                    loggingQueue_.pop_front();
                    if (loggingQueue_.empty())
                        break;
                    continue;
                }

                tmpOut += std::get<1>(line);
                loggingQueue_.pop_front();
                if (loggingQueue_.empty())
                    break;
            }
        }

        outStreamFile_ << tmpOut << std::flush;
        currentLine_ += writtenLine;
        
        if (currentLine_ > maxfileline_)
        {
            ChangeFile();
            currentLine_ = 0;
        }
    }
}

void Logger::SetSeverity(const std::string& severity)
{
    logoverseverity_=StringToSeverity(severity);
}

Logger::~Logger()
{
    continueLogging_=false;
    threadHandler_->join();
}