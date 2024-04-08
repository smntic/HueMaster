cmake_minimum_required(VERSION 3.27)
project(HueMaster)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

find_package(OpenCV REQUIRED)

add_executable(HueMaster src/main.cpp
        src/image.cpp
        include/image.h
        src/color.cpp
        include/color.h
        src/color_scheme.cpp
        include/color_scheme.h
)

target_link_libraries(HueMaster ${OpenCV_LIBS})
