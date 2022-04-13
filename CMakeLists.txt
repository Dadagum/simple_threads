cmake_minimum_required(VERSION 3.5)
project (simple_threads)


set(PROJECT_SOURCE_DIR .)
set(CMAKE_BINARY_DIR ./build )
add_compile_options(-Wall -W -g)
add_definitions(-DDEBUG)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/threads DIR_THREADS_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils DIR_UTILS_SRCS)

add_library(simple_threads SHARED
    ${DIR_THREADS_SRCS}    
    ${DIR_UTILS_SRCS}
)

target_include_directories(simple_threads
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src/include
) 

# link pthread
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(simple_threads Threads::Threads)

# test
add_executable(test_thread_pool
    ${PROJECT_SOURCE_DIR}/src/test/test_thread_pool.cpp
)

target_include_directories(test_thread_pool
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src/include
) 

target_link_libraries( test_thread_pool
    PRIVATE
        simple_threads
)

