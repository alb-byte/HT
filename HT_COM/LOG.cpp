#include "pch.h"
#include "LOG.h"
#include <WinBase.h>

void Logger::Write(std::string msg) {
	char timeBuffer[80]{ 0 };
	time_t seconds = time(NULL);
	struct tm timeinfo;
	localtime_s(&timeinfo, &seconds);
	const char* format = "%e.%m.%Y %I:%M:%S";
	strftime(timeBuffer, 80, format, &timeinfo);
	std::ofstream out("D:\\OS\\COM.log", std::ios::app);
	out <<
		std::left << std::setw(25) << timeBuffer <<
		std::internal << std::setw(4) << "::" <<
		std::right << std::setw(30) << msg << std::endl;
	out.close();
}
void Logger::Write(std::string msg, void* parm) {
	char timeBuffer[80]{ 0 };
	time_t seconds = time(NULL);
	struct tm timeinfo;
	localtime_s(&timeinfo, &seconds);
	const char* format = "%e.%m.%Y %I:%M:%S";
	strftime(timeBuffer, 80, format, &timeinfo);
	std::ofstream out("D:\\OS\\COM.log", std::ios::app);
	out <<
		std::left << std::setw(25) << timeBuffer <<
		std::internal << std::setw(4) << "::" <<
		std::right << std::setw(30) << msg <<
		std::right << std::setw(30) << parm <<
		std::endl;
	out.close();
}
