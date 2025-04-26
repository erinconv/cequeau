// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _LINUX
#include "targetver.h"
#endif

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

//#include <octave/oct.h>
// Includes STL
#include <algorithm>
#include <cstring>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <climits>
#include <map>
#include <sstream>
#include <vector>
#include <ctime>
#include <chrono>
#include <array>
#include <memory>

#include <string>

#include "DateChrono.h"

// Logger
#include "log.h"
// #include "csv_logger.h"

// #include "Util.h"

#ifdef ENV_MATLAB
    #include <mat.h>
#endif

#include "MexHelper.hpp"



typedef int matError;

#define minf(float_a, float_b) std::min<float>(float_a,float_b)
#define maxf(float_a, float_b) std::max<float>(float_a,float_b)
#define absf(float_a) std::abs<float>(float_a)

const float PI = 3.1416f;
const float FUSEAU_HORAIRE = -4.0f;
