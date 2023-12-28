cmake_minimum_required(VERSION 3.22.1)

project(churchill)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON REQUIRED jansson)

include_directories(${CURL_INCLUDE_DIR})
include_directories(${JSON_INCLUDE_DIRS})
include_directories(
    include
    include/DeltaExchange
    include/DeribitExchange
    include/PutCallParity
    include/util)

file(GLOB SOURCES 
    src/*.cpp
    src/DeltaExchange/*.cpp
    src/DeribitExchange/*.cpp
    src/PutCallParity/*.cpp
    src/util/*.cpp)

add_executable(churchill ${SOURCES})

target_link_libraries(churchill ${CURL_LIBRARIES} ${JSON_LIBRARIES})

find_program(VALGRIND_EXECUTABLE valgrind)
if(VALGRIND_EXECUTABLE)
    message("Valgrind found: ${VALGRIND_EXECUTABLE}")
    set(VALGRIND "valgrind --leak-check=full --track-origins=yes")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_VALGRIND")
else()
    message("Valgrind not found")
endif()
