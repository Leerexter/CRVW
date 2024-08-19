cmake_minimum_required(VERSION 3.5.0)
project(testCmake VERSION 0.1.0 LANGUAGES C CXX)

add_executable(testCmake main.cpp)

include(CTest)
enable_testing()

find_package( OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(testCmake ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
