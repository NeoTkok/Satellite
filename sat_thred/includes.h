//
// Created by AS on 3/11/2022.
//


//warning do not include serv.h file!

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>


#include <iostream>
using std::cout;
using std::cin;
#include "satellite.h"
#include "vector"
using std::vector;
#include "cmath"
using std::sqrt;
using std::pow;
#include "integrator.h"
#include <chrono>
#include <thread>
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
