cmake_minimum_required(VERSION 4.1)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)


add_executable(untitled src/cpp/main.cpp
        src/cpp/render.cpp)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)