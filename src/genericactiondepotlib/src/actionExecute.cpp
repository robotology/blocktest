/******************************************************************************
 *                                                                            *
 * Copyright (C) 2021 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionExecute.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#include "actionExecute.h"

#include <cstddef>
#include <sstream>

#include "logger.h"
#include "report.h"
#include "testsDepot.h"

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionExecute, "execute");

namespace GenericActions
{
std::map<std::string, std::shared_ptr<boost::process::child>> ActionExecute::processes_;

ActionExecute::ActionExecute(const CommandAttributes& commandAttributes, const std::string& testCode) : Action(commandAttributes, testCode)
{
}

void ActionExecute::beforeExecute()
{
	getCommandAttribute("tag", tag_);
	getCommandAttribute("command", commandName_);
	getCommandAttribute("param", commandParam_);
	getCommandAttribute("prefix", prefix_);
	getCommandAttribute("waitafter", waitafter_);
	getCommandAttribute("kill", kill_);
	getCommandAttribute("nobackground", nobackground_);
	getCommandAttribute("usetestpath", useTestPath_);
	getCommandAttribute("writetofile", writeToFile_);
	getCommandAttribute("waitforend", waitForEnd_);
	getCommandAttribute("expectedexitcode", expectedExitCode_);
}

execution ActionExecute::execute(const TestRepetitions& testrepetition)
{
	std::string tagTmp = normalizeSingle(tag_, false);
	if (kill_)
	{
		if (processes_.find(tagTmp) == processes_.end())
		{
			std::stringstream ss;
			ss << "tag " << tagTmp << " not found for kill." << std::endl;
			addProblem(testrepetition, Severity::critical, ss.str(), true);
			return execution::stopexecution;
		}

		auto process = processes_[tagTmp];
		process->terminate();
		processes_.erase(tagTmp);
		std::this_thread::sleep_for(std::chrono::seconds(waitafter_));
		return execution::continueexecution;
	}
	std::stringstream ss;
	if (useTestPath_)
		ss << TestsDepot::path_ << "/";
	if (!prefix_.empty())
		ss << prefix_ << " ";

	ss << commandName_ << " " << normalize(commandParam_, false);

	if (processes_.find(tagTmp) != processes_.end())
	{
		TXLOG(Severity::warning) << "Repetition " << testrepetition.testRepetitions_ << ": duplicate execute command tag " << tagTmp << std::endl;
	}

	TXLOG(Severity::debug) << "Executing:" << ss.str() << std::endl;
	try
	{
		std::string writeToFileTmp = normalizeSingle(writeToFile_, false);
		std::shared_ptr<boost::process::child> process{nullptr};
		if (!writeToFile_.empty())
		{
			process = std::make_shared<boost::process::child>(ss.str(), boost::process::std_out > writeToFileTmp, boost::process::std_err > writeToFileTmp);
		}
		else
		{
			process = std::make_shared<boost::process::child>(ss.str());
		}

		if (waitForEnd_)
		{
			process->wait();
			int result = process->exit_code();
			if (result != expectedExitCode_)
			{
				std::stringstream sstmp;
				sstmp << "Wrong exit code:" << result << " Expected:" << expectedExitCode_ << std::endl;
				addProblem({0, 0}, Severity::critical, sstmp.str(), true);
			}
		}
		else
		{
			processes_[tagTmp] = process;
		}
	}
	catch (const std::exception& e)
	{
		TXLOG(Severity::error) << "Execution " << e.what() << std::endl;
		TXLOG(Severity::error) << "Problem with " << ss.str() << std::endl;
		return execution::continueexecution;
	}

	std::this_thread::sleep_for(std::chrono::seconds(waitafter_));
	return execution::continueexecution;
}

}  // namespace GenericActions