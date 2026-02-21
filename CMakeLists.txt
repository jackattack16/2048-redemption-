cmake_minimum_required(VERSION 3.20)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

set(SFML_STATIC_LIBRARIES FALSE)
find_package(SFML 3.0 COMPONENTS Graphics REQUIRED)

add_executable(untitled src/cpp/main.cpp
        src/headers/render.h
        src/headers/roundedRectangle.h
        src/cpp/cubicBezier.cpp
        src/headers/cubicBezier.h
        src/cpp/scoreNumberEffect.cpp
        src/headers/scoreNumberEffect.h)

target_link_libraries(untitled PRIVATE SFML::Graphics)

# Copy font file to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/headers/Blue\ Winter.ttf
     DESTINATION ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")