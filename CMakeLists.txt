cmake_minimum_required(VERSION 3.2)
project(chess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2")

include_directories(source/geometry)
include_directories(source/logic)

add_library(geometry SHARED source/geometry/gvector.cpp)
add_library(piece SHARED source/logic/piece.cpp)
add_library(move SHARED source/logic/move.cpp)
add_library(position SHARED source/logic/position.cpp)
add_library(board SHARED source/logic/board.cpp)

target_link_libraries(move geometry)
target_link_libraries(piece move)
target_link_libraries(position piece)
target_link_libraries(board position)

add_executable(CHESS source/main.cpp)
add_executable(CHESS_TEST source/main_test.cpp source/tests.cpp)
add_executable(ENGINE source/engine.cpp)

find_package(GTest REQUIRED) 
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(CHESS board)
target_link_libraries(CHESS_TEST board ${GTEST_LIBRARIES} pthread)
target_link_libraries(ENGINE board)

