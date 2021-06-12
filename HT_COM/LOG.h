#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>

class Logger {
public:
	static void Write(std::string msg);
	static void Write(std::string msg, void* parm);
};

