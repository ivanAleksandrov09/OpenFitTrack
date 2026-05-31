cmake_minimum_required(VERSION 3.20)
project(FitnessTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(FitnessTracker ${SOURCES})

if(WIN32)
	target_compile_definitions(FitnessTracker PRIVATE _WIN32_WINNT=0x0600)
endif()