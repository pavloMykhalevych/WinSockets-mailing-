#pragma once

#include <winsock2.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <ctime>
#include <chrono>
#include <time.h>
#include <stdio.h>

void SendString(std::string mes, SOCKET Connection);

const std::string currentDateTime();