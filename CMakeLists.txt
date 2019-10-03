cmake_minimum_required(VERSION 3.13)

project(iml VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

# use c++17
# --------------------------------------- #
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -O3)
add_definitions(-D_LIBCPP_DISABLE_AVAILABILITY)

# executables
# --------------------------------------- #
file(GLOB_RECURSE iml_src
    "src/*.cpp"
)
add_executable(iml ${iml_src})

target_include_directories(iml PRIVATE include/)
