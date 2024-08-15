cmake_minimum_required(VERSION 3.28)
project(chirpParameters)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

file(GLOB SRC "*.cpp")

# executable profile
# add_executable(${CMAKE_PROJECT_NAME} ${SRC})

# static lib profile
#file(GLOB REMOVE_SOURCES "example.cpp")
#list(REMOVE_ITEM SRC ${REMOVE_SOURCES})
#add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC})

# dynamic lib profile
file(GLOB REMOVE_SOURCES "example.cpp")
list(REMOVE_ITEM SRC ${REMOVE_SOURCES})
add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC})
