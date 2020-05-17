cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)

project(uSynth LANGUAGES CXX)

set(SOURCES
    src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})


# use system installed rtaudio
# I can never get a submodule to work
find_path(RT_INCLUDE_DIR NAMES rtaudio/rtaudio.h)
find_library(RT rtaudio REQUIRED)
message(STATUS "RT: [${RT} ${RT_INCLUDE_DIR}]")
target_link_libraries(${PROJECT_NAME} ${RT})
target_include_directories(${PROJECT_NAME} PUBLIC ${RT_INCLUDE_DIR})

