cmake_minimum_required(VERSION 3.12.1)
project(cpp_tools)

include_directories(${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_LIST_DIR}/*.cc)
file(GLOB_RECURSE REMOVE_TEST_CPP ${CMAKE_CURRENT_LIST_DIR}/*_test.cc)

foreach(ITEM ${REMOVE_TEST_CPP})
    list(REMOVE_ITEM SRCS ${ITEM})
endforeach()

add_library(${PROJECT_NAME} STATIC ${SRCS})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
