cmake_minimum_required(VERSION 3.31)
project(boundedQueue)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(BoundedQueue STATIC src/BoundedQueue.cpp)

target_include_directories(BoundedQueue
        PRIVATE
            src/BoundedQueue.cpp
        PUBLIC
        include/BoundedQueue.h
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests/tests.cpp)

target_link_libraries(tests PRIVATE BoundedQueue gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)