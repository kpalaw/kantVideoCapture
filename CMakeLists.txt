# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.0)

# Set the project name and version
project(video2 VERSION
        1.0
        DESCRIPTION "CMake kant"
        LANGUAGES CXX)

# Add all the source files in the current directory to the "sources" variable
#file(GLOB sources "*.cpp")

# Force using g++ compiler
set(CMAKE_CXX_COMPILER g++)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -lavformat -lavcodec -lavutil -lswscale -lpng -lSDL2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g ")

# Add an executable target called "my_executable"
add_executable(kantcap 
               sources/kantcap.cpp
               sources/progressBar.cpp)

find_library(LIBAVFORMAT_LIBRARY avformat)
target_link_libraries(kantcap ${LIBAVFORMAT_LIBRARY})    

find_library(LIBAVCODEC_LIBRARY avcodec)
target_link_libraries(kantcap ${LIBAVCODEC_LIBRARY}) 

find_library(LIBAVUTIL_LIBRARY avutil)
target_link_libraries(kantcap ${LIBAVUTIL_LIBRARY})

find_library(LIBPNG_LIBRARY png)
target_link_libraries(kantcap ${LIBPNG_LIBRARY})

find_library(LIBSDL2_LIBRARY SDL2)
target_link_libraries(kantcap ${LIBSDL2_LIBRARY})

find_library(LIBSWSCALE_LIBRARY swscale)
target_link_libraries(kantcap ${LIBSWSCALE_LIBRARY})



message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")


