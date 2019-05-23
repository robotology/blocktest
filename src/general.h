#pragma once

#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <chrono>
#include <memory>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <set>
#include <list>
#include <queue>
#include <algorithm>
#include <fstream>
#include <mutex>
#include <experimental/filesystem>

#ifndef _WIN32
#include <dlfcn.h>
#endif // _WIN32

#if defined(WIN32) || defined(_WIN32)
#define BLOCKTEST_EXPORT __declspec(dllexport)
#else
#define BLOCKTEST_EXPORT [[gnu::visibility("default")]]
#endif

#include <boost/process.hpp>
#include <boost/dll/import.hpp>   
#include <boost/dll/shared_library.hpp> 
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>

#include "pugixml.hpp"
