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
#include <iterator>
#include <cmath>
#include <cassert>

#define BLOCKTEST_UNUSED(x) (void)x;


constexpr char maintestfile[] = "./test/test.xml";

std::string calcolateTestName(const std::string& name,const std::string& path);