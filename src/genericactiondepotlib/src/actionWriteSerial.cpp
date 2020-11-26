/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionWriteSerial.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */
#if not defined WIN32

#include "actionWriteSerial.h"
#include "logger.h"
#include "tables.h"
#include "report.h"

#include <string>

#ifdef COMPILE_WITHEXTSERIAL
#include "serial/serial.h"
#else
#include <string.h>
#include <stdio.h>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#endif

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionWriteSerial,"writeserial");

namespace GenericActions
{

ActionWriteSerial::ActionWriteSerial(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
}  

void ActionWriteSerial::beforeExecute()
{
    getCommandAttribute("port",port_);
    getCommandAttribute("value",value_);
    getCommandAttribute("baud",baud_);
}

#ifndef COMPILE_WITHEXTSERIAL

execution ActionWriteSerial::execute(const TestRepetitions&)
{
    int serial_port = open(port_.c_str(), O_RDWR);

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0) {
        TXLOG(Severity::error)<<errno<<"from tcgetattr:"<<strerror(errno)<<" Port:"<<port_<<std::endl;
        return execution::continueexecution;        
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set out baud rate
    unsigned int baudToBeAssigned=bauddepot_["9600"];
    auto it=bauddepot_.find(baud_);
    if(it!=bauddepot_.end())
    {
        baudToBeAssigned=it->second;
    }
    cfsetospeed(&tty, baudToBeAssigned);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        TXLOG(Severity::error)<<errno<<"from tcsetattr:"<<strerror(errno)<<" Port:"<<port_<<std::endl;
        return execution::continueexecution;
    }

    // Write to serial port
    unsigned char msg[] = { static_cast<unsigned char>(value_.c_str()[0]) };
    write(serial_port, msg, sizeof(msg));
    TXLOG(Severity::debug)<<"Set value:"<<value_<<" To:"<<port_<<std::endl;
    return execution::continueexecution;
}
#else
execution ActionWriteSerial::execute(const TestRepetitions& testrepetition)
{
    //JUST A TEST
    serial::Serial my_serial(port_, 9600, serial::Timeout::simpleTimeout(1000));

    if(my_serial.isOpen())
        TXLOG(Severity::debug)<<"Serial port opened port:"<<port_<<std::endl;
    else
        TXLOG(Severity::error)<<"Serial port not opened port:"<<port_<<std::endl;

    //TODO WRITE ON PORT
    return execution::continueexecution;
}
#endif

}
#endif // WIN32
