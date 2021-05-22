cmake_minimum_required(VERSION 3.15...3.20)

project(on-line-chatting-service LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(CMAKE_BUILD_TYPE Debug   CACHE STRING "-g"           FORCE)
# set(CMAKE_BUILD_TYPE Release CACHE STRING "-O3 -DNDEBUG" FORCE)
# set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "-O2 -g" FORCE)

# for ale
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(apps)

