cmake_minimum_required(VERSION 3.0.0)
project(open-cv-t2 VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(open-cv-t2 main.cpp polar.cpp vidcap.cpp hog.cpp blurCapture.cpp frameDifferencing.cpp bgSub.cpp smask.cpp conv.cpp wowfilter.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
