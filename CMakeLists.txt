cmake_minimum_required(VERSION 3.0.0)

project(Blast VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_BUILD_TYPE "Debug")

file(GLOB_RECURSE  code_SRC
     "src/*.h"
     "src/*.cpp"
     "src/*.hpp"
)

add_executable(blast ${code_SRC})

target_include_directories(blast PRIVATE include)
target_link_directories(blast PRIVATE lib)
target_link_libraries(blast glew32s glfw3 opengl32 gdi32)
add_compile_definitions(GLEW_STATIC BLAST_WIN)
