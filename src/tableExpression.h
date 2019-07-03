/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file tableexpression.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "api.h"
#include "general.h"
#include "table.h"

#include "exprtk.hpp"

namespace BlockTestCore
{

class TableExpression: public Table
{ 
    public:
        bool Init(std::vector<std::string> tableValue) override;

        std::string get() override;
        std::string fetch() override;
        std::string get(unsigned int position) const override;

    private:
        double x_{0};
        exprtk::expression<double> expression_;
        exprtk::symbol_table<double> symbolTable_;
        exprtk::parser<double> parser_;

        double incrementValue_{0};
        double start_{0};
        double stop_{0};        
};
}