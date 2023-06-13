cmake_minimum_required(VERSION 3.0.0)

project(Blast VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_BUILD_TYPE "Release")

file(GLOB_RECURSE  code_SRC
     "src/*.cpp"
     "src/*.c"
     "src/*.hpp"
     "src/*.h"
)

add_executable(blast ${code_SRC})

# disable window console
target_link_options(blast PRIVATE "-mwindows")

target_include_directories(blast PRIVATE include)
target_link_directories(blast PRIVATE lib)
target_link_libraries(blast glfw3 opengl32 gdi32)
add_compile_definitions(BLAST_WIN)


