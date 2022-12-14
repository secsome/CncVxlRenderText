#pragma once
//basic
#include <algorithm>
#include <filesystem>
#include <ctime>

//streams
#include <fstream>
#include <iostream>

//containers
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <cassert>
#include <cstring>
#include <cmath>

static_assert(sizeof(size_t) == 4, "This program can be only compiled under 32bit mode!");

// Ensure sizeof(float_t) = 4
using float32_t = float;

#ifndef WINAPI

#ifdef _MSC_VER
	#define WINAPI __stdcall
#endif

#ifdef __GNUC__ 
	#define WINAPI __attribute__((__stdcall__))
#endif

#endif

using byte = uint8_t;

#define CLASSES_START namespace thomas{
#define CLASSES_END };

#include "d3dx9.hpp"