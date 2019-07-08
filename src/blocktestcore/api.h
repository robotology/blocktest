/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

#pragma once

#if defined(WIN32) || defined(_WIN32)
#define BLOCKTEST_EXPORT __declspec(dllexport)
#else
#define BLOCKTEST_EXPORT [[gnu::visibility("default")]]
#endif

