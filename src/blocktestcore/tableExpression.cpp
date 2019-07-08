/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file tableexpression.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#include "tables.h"
#include "logger.h"

#include "tableExpression.h"

namespace BlockTestCore
{

bool TableExpression::Init(std::vector<std::string> table)
{
    bool out = Table::Init(table);

    incrementValue_ = std::atof(table_[3].c_str());
    start_ = std::atof(table_[4].c_str());
    stop_ = std::atof(table_[5].c_str());

    symbolTable_.add_variable("x", x_);
    symbolTable_.add_constants();
    std::string expression_string = table_[6];
    expression_.register_symbol_table(symbolTable_);
    parser_.compile(expression_string, expression_);
    x_ = start_;
    return out;
}

std::string TableExpression::get()
{
    double y = expression_.value();
    x_ += incrementValue_;

    if (x_ >= stop_)
        x_ = start_;

    return std::to_string(y);
};

std::string TableExpression::fetch()
{
    auto out = table_[currentPosition_];
    return out;
};

std::string TableExpression::get(unsigned int position) const
{
    if (position > table_.size() - 1)
    {
        TXLOG(Severity::error) << "Wrong position for table" << std::endl;
        return "";
    }
    return table_[currentPosition_];
}

} // namespace BlockTestCore
