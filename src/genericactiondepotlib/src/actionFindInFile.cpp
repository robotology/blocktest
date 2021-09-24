/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionPrint.cpp
 * @author Luca Tricerri
 */

#include "actionFindInFile.h"

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
			}
			else
				TXLOG(Severity::info) << "String:" << string_ << " find in:" << filename_ << std::endl;
		}
	}

	return execution::continueexecution;
}
}  // namespace GenericActions
