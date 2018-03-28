cmake_minimum_required(VERSION 3.8)
project(User_Threads)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES my_pthread.c my_pthread.h Queue.c Queue.h frame.c frame.h page.c page.h myallocate.c myallocate.h)
add_executable(User_Threads ${SOURCE_FILES})