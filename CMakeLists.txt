cmake_minimum_required(VERSION 3.5)
project (simple_threads)

set(PROJECT_SOURCE_DIR .)
add_compile_options(-Wall -W -ggdb -std=c++11)
add_definitions(-DDEBUG)

# test
add_executable(test_thread_pool
    ${PROJECT_SOURCE_DIR}/src/test/test_thread_pool.cpp
)

target_include_directories(test_thread_pool
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src/include
) 

# link pthread
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(test_thread_pool Threads::Threads)

