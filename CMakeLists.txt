cmake_minimum_required(VERSION 3.17)
project(PinocchioSql)

set(CMAKE_CXX_STANDARD 20)


add_library(${PROJECT_NAME} pinocchioQB.cpp pinocchioQB.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")
set(PQXX /usr/local/include/pqxx)
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)
target_link_libraries(${PROJECT_NAME} ${PQXX_LIB} ${PQ_LIB})