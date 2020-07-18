/*
 * File: Logger.h
 */
#ifndef _LOGGER_H
#define _LOGGER_H

#include<mutex>
#include<fstream>

#define LOGMSGSIZE 128

class Logger {
	private:
		static std::mutex _mutex;
		static std::ofstream _filestream;
	public:
		static void LogMsg(const char* msg);
};

#endif
