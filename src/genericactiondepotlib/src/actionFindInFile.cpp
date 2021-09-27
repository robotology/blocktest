/******************************************************************************
 *                                                                            *
 * Copyright (C) 2021 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionFindInFIle.cpp
 * @author Luca Tricerri
 */

#include "actionFindInFile.h"

#include <chrono>
#include <filesystem>

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionFindInFile, "findinfile");

namespace GenericActions
{
ActionFindInFile::ActionFindInFile(const CommandAttributes &commandAttributes, const std::string &testCode) : Action(commandAttributes, testCode)
{
}

void ActionFindInFile::beforeExecute()
{
	getCommandAttribute("filename", filename_);
	getCommandAttribute("string", string_);
	getCommandAttribute("erroronfind", errorOnFind_);
	getCommandAttribute("bckiferror", bckIfError_);
}

execution ActionFindInFile::execute(const TestRepetitions &)
{
	std::ifstream ifs;
	ifs.open(filename_);
	if (!ifs.is_open())
	{
		TXLOG(Severity::error) << "Find in file wrong file name:" << filename_ << std::endl;
		return execution::continueexecution;
	}

	std::string currentLine;
	while (getline(ifs, currentLine))
	{
		if (currentLine.find(string_, 0) != std::string::npos)
		{
			if (errorOnFind_)
			{
				TXLOG(Severity::error) << "String:" << string_ << " find in:" << filename_ << std::endl;
				TXLOG(Severity::error) << "Line:" << currentLine << std::endl;
				TestRepetitions rep{0, 0};
				addProblem(rep, Severity::critical, "String find in file:" + string_, true);

				if (bckIfError_)
				{
					std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					char buf[100] = {0};
					std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H:%M:%S", std::localtime(&now));
					std::stringstream ss;
					ss << filename_ << "_" << buf << ".bck";
					std::filesystem::copy(filename_, ss.str(),std::filesystem::copy_options::overwrite_existing);
				}
			}
			else
				TXLOG(Severity::info) << "String:" << string_ << " find in:" << filename_ << std::endl;
		}
	}

	return execution::continueexecution;
}
}  // namespace GenericActions
