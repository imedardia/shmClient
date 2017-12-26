cmake_minimum_required(VERSION 2.6)
project (ShmTest)

set (SHM_SRC   shm_test.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-varargs")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -O3")

#Check if Boost is available on system
find_package(Boost 1.40.0 COMPONENTS date_time filesystem system)
if (NOT ${Boost_FOUND})
    message( FATAL_ERROR "libboost-all-dev was not found : ${Boost_NOT_FOUND_MESSAGE}" )
else ()
	include_directories(${Boost_INCLUDE_DIRS})
endif(NOT ${Boost_FOUND})

#compiling step
add_executable(ShmTest ${SHM_SRC} ${Boost_INCLUDE_DIRS})

#link step
target_link_libraries(ShmTest ${Boost_LIBRARIES} -lboost_system -lpthread -lrt)

