cmake_minimum_required(VERSION 2.8.3)

project (object_tracking)


find_package(OpenCV REQUIRED)

include_directories(./cvBlob)
#include_directories(./inc)
INCLUDE_DIRECTORIES(${OPENCV_INCLUDE_DIR})

add_executable (trackObjects src/red_object_tracking.cpp)


  TARGET_LINK_LIBRARIES(trackObjects ${OPENCV_LIBRARIES})
