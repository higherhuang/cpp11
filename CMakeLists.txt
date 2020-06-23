cmake_minimum_required (VERSION 2.8)
project(cpp11)

if (NOT MSVC)
#set(CFLAGS "-m64")
#添加c++17支持
add_definitions(-std=c++17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -std=c++17 -pthread -ldl ")
else (NOT MSVC)
#添加c++11支持
add_definitions(-std=c++11)
add_definitions(-D WIN32_LEAN_AND_MEAN)
endif (NOT MSVC)

set(CMAKE_INSTALL_PREFIX "./")


add_subdirectory(include)
add_subdirectory(IocContainerTest)