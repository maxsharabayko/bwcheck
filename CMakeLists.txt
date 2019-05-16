cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(BWCHECK C CXX)


add_subdirectory(submodule/CLI11)
# Adds ${CLI11_SOURCE_DIR} etc.

if (NOT WIN32 AND NOT MINGW)
	find_package(Threads REQUIRED)
	set(PTHREAD_LIBRARY ${CMAKE_THREAD_LIBS_INIT})
	find_package(Boost 1.70 COMPONENTS date_time regex REQUIRED )
elseif(WIN32)
	set (Boost_THREADAPI win32)
	set (Boost_COMPILER "-vc")
	set (Boost_USE_STATIC_RUNTIME ON)
	find_package(Boost 1.70)
endif()


#SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "D:/Projects/boost/boost_1_70_0/boost")
#SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/Projects/boost/boost_1_70_0/stage/lib")


#find_package(Boost 1.70 COMPONENTS date_time regex REQUIRED )
#find_package(Boost 1.70)
#find_package(date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_INCLUDE_DIR}/stage/lib)

add_subdirectory(bwchecker)


