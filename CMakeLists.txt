cmake_minimum_required(VERSION 3.28)
project(CFG)

set(CMAKE_CXX_STANDARD 17)

add_executable(CFG main.cpp
        Sources/CFG.cpp
        Sources/PDA.cpp
        Sources/Logger.cpp
)
