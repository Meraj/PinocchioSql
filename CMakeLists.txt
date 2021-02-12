cmake_minimum_required(VERSION 3.17)
project(PinocchioSql)

set(CMAKE_CXX_STANDARD 20)

add_library(PinocchioSql querybuilder.cpp querybuilder.h)