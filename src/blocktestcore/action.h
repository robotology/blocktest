/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file action.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "api.h"
#include "general.h"
#include "logger.h"
#include "type.h"

using CommandAttributes = std::map<std::string, std::string>;

#include "actionRegister.h"
#include "testrepetitions.h"

namespace BlockTestCore
{
/**
 * @class Action
 * @brief ...
 * @todo missing brief, detailed description and methods documentation
 */

class BLOCKTEST_EXPORT Action
{
   public:
	Action(const CommandAttributes& commandAttributes, const std::string& testCode);
	virtual void beforeExecute(){};
	virtual execution execute(const TestRepetitions& testrepetition) = 0;
	virtual void afterExecute(){};
    virtual void afterExecuteAllRepetitions(){};
	virtual double getDouble()
	{
		return 0;
	};
	virtual ~Action();

   protected:
	std::string normalize(const std::string& str, bool justFetch) const;
	std::string normalizeSingle(const std::string& str, bool justFetch) const;
	double normalizeDouble(const std::string& str, bool justFetch) const;
	int normalizeInt(const std::string& str, bool justFetch) const;
	unsigned int normalizeUInt(const std::string& str, bool justFetch) const;

	void addProblem(const TestRepetitions& repetitions, Severity severity, const std::string& errorMessage, bool alsoLog) const;
	std::string testCode_;

	bool reporterror_{true};

	// helper funcion
	virtual void getCommandAttribute(const std::string& name, std::string& out) const;
	virtual void getCommandAttribute(const std::string& name, unsigned int& out) const;
	virtual void getCommandAttribute(const std::string& name, int& out) const;
	virtual void getCommandAttribute(const std::string& name, double& out) const;
	virtual void getCommandAttribute(const std::string& name, bool& out) const;

   private:
	const CommandAttributes commandAttributes_;

   public:
	// helper funcion
	template <typename T>
	static void tokenize(const std::string& toTokenize, std::vector<T>& out)
	{
		std::istringstream ss{toTokenize};
		out = std::vector<T>{std::istream_iterator<T>{ss}, std::istream_iterator<T>()};
	}

	template <typename T>
	static std::vector<T> tokenize(const std::string& toTokenize)
	{
		std::vector<T> out;
		std::istringstream ss{toTokenize};
		out = std::vector<T>{std::istream_iterator<T>{ss}, std::istream_iterator<T>()};
		return out;
	}
};

}  // namespace BlockTestCore
