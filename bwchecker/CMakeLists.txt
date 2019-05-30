cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(bwcheck)
set (CMAKE_CXX_STANDARD 11)
FILE(GLOB SOURCES *.cpp *.hpp)


add_executable(bwcheck ${SOURCES})

#target_link_libraries(LINK_PUBLIC CLI11::CLI11)
#target_link_libraries(bwcheck ${Boost_LIBRARIES} CLI11::CLI11)
target_link_libraries(bwcheck Boost::boost CLI11::CLI11 ${PTHREAD_LIBRARY})
