/*
 * File: Logger.cpp
 */

#include<Logger.h>

std::mutex Logger::_mutex;
std::ofstream Logger::_filestream("windowstovmproxy.log");

void Logger::LogMsg(const char* msg) {
	std::lock_guard<std::mutex> lock(_mutex);
	_filestream <<msg <<std::endl;
}
