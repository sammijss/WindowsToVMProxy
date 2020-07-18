message(STATUS "Project 'windows-to-vm-proxy' by Sammi Kumar Srivastava")

cmake_minimum_required(VERSION 2.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(inc) #To include header files

project(windows-to-vm-proxy)

add_executable (
	windowstovmproxy	#Name of the executable
 	src/main.cpp		#Name of the source file
	src/Logger.cpp
	src/UDPProxy.cpp
)
