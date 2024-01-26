cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(nex)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the necessary source files
add_executable(nex MACOSX_BUNDLE nex.cpp
        depend/cell.h
        depend/gmshEX.h
        depend/noiseEx.h
        depend/Point.h
        depend/remesh.h
        depend/subCell.h
        depend/noiseEx.cpp
        depend/Point.cpp
        depend/subCell.cpp
        depend/cell.cpp
        VTKParser/depend/VTKParser.cpp
        depend/argparse.hpp
)

# Include the directories with header files
target_include_directories(nex PRIVATE depend VTKParser/depend)


