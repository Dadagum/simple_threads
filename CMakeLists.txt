cmake_minimum_required(VERSION 3.5)
project (simple_threads)

set(CMAKE_BINARY_DIR 
    $(CMAKE_SOURCE_DIR)/build
)

set(PROJECT_SOURCE_DIR ./src)

aux_source_directory(${PROJECT_SOURCE_DIR}/threads DIR_THREADS_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/utils DIR_UTILS_SRCS)

add_library(simple_threads SHARED
    ${DIR_THREADS_SRCS}    
    ${DIR_UTILS_SRCS}
)

target_include_directories(simple_threads
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
) 