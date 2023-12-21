cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME churchill)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

# Find CURL library
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

# Find Jansson library
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON REQUIRED jansson)
include_directories(${JSON_INCLUDE_DIRS})

# Add executable
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES} ${JSON_LIBRARIES})
