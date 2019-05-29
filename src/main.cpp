/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file blockTest.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "blockTest.h"


int main(int argc,char* argv[]) noexcept
{    
    try
    {
        std::string path;
        if(argc>1)
            path=argv[1];

        BlockTest test(path);
        bool out=test.run();
        return out;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}
