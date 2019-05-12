cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(BWCHECK C CXX)


add_subdirectory(submodule/CLI11)
# Adds ${CLI11_SOURCE_DIR} etc.

set (Boost_THREADAPI win32)
set (Boost_COMPILER "-vc")
set (Boost_USE_STATIC_RUNTIME ON) 
#find_package(Boost 1.70 COMPONENTS date_time regex REQUIRED )
find_package(Boost 1.70)
#find_package(date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_INCLUDE_DIR}/stage/lib)

add_subdirectory(bwchecker)


