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

ACTIONREGISTER_DEF_TYPE(ActionUpdateFile,"updatefile");

ActionUpdateFile::ActionUpdateFile(const pugi::xml_node& nodeCommand,Test_sptr test):Action(nodeCommand,test)
{
    sourceFile_ =nodeCommand.attribute("sourcefile").value();
    destinationFile_ =nodeCommand.attribute("destinationfile").value();
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
