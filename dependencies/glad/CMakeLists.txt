cmake_minimum_required(VERSION 3.10)

project(glad LANGUAGES CXX VERSION 3.3)

add_library(glad STATIC ${PROJECT_SOURCE_DIR}/src/glad.c)

target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/include)
