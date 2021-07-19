cmake_minimum_required(VERSION 3.0.0)
project(vigipred VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#include(CTest)
#enable_testing()

# add executable(s)
add_executable(vigipred main.cpp src/Grid.h src/Random.h src/Individual.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

