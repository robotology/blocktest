/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionUpdateFile.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionUpdateFile.h"
#include "logger.h"

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionUpdateFile,"updatefile");

namespace GenericActions
{


ActionUpdateFile::ActionUpdateFile(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
    getCommandAttribute(commandAttributes,"sourcefile",sourceFile_);
    getCommandAttribute(commandAttributes,"destinationfile",destinationFile_);    
}     

bool ActionUpdateFile::execute(unsigned int testrepetition)
{
    std::ifstream ifs(sourceFile_);
    if(!ifs.is_open())
    {
        TXLOG(Severity::error)<<"source file not found:"<<sourceFile_<<std::endl;
        return false;
    }
    std::ofstream ofs(destinationFile_,std::fstream::in | std::fstream::trunc);
    if(!ofs.is_open())
    {
        TXLOG(Severity::error)<<"destination file not ready:"<<destinationFile_<<std::endl;
        return false;
    }    

    std::string str((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
    str=normalize(str,false);

    ofs<<str;

    return true;
}

}