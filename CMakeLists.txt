#cmake for qt project

cmake_minimum_required(VERSION 3.5)

project(minesweeper)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_BUILD_TYPE)

option(set_clang "set clang as default compiler" 0)

if(set_clang)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    message(STATUS "Use option set_clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif(set_clang)

message(STATUS "buid type    " ${CMAKE_BUILD_TYPE})
message(STATUS "Project      " ${PROJECT_NAME})
message(STATUS "c compiler   " ${CMAKE_C_COMPILER})
message(STATUS "cxx compiler " ${CMAKE_CXX_COMPILER})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)

include_directories("./include")

set(MAIN_SRC sources/main.cpp)
set(MINES_LIB "mines")
file(GLOB MINES_SRC "sources/mines/*.cpp" "include/*.hpp")
set(IMAGES_SRC "images.qrc")

add_executable(${PROJECT_NAME} ${MAIN_SRC} ${IMAGES_SRC})
add_library(${MINES_LIB} SHARED ${MINES_SRC})

target_link_libraries(${PROJECT_NAME} ${MINES_LIB} Qt5::Widgets)
target_link_libraries(${MINES_LIB} Qt5::Widgets)
