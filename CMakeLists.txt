cmake_minimum_required(VERSION 3.10)

# set the project name
project(enum8)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# To enable debugging build; A lot of debugging information is going to be printed out
add_compile_definitions(DEBUG)

# add the executable
add_executable(enum8 Main.cpp Memory.cpp Cpu.cpp Gfx.cpp Keyboard.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lSDL2)
