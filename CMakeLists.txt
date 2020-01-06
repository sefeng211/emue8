cmake_minimum_required(VERSION 3.10)

# set the project name
project(emue8)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(DEBUGGING_MESSAGE "Enable Debugging Messages" OFF) #OFF by default
# To enable debugging build; A lot of debugging information is going to be printed out
if (DEBUGGING_MESSAGE)
	add_compile_definitions(DEBUG)
endif(DEBUGGING_MESSAGE)

# add the executable
add_executable(emue8 Main.cpp Memory.cpp Cpu.cpp Gfx.cpp Keyboard.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lSDL2)
TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)
