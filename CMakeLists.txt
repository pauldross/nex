cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(nex)

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
        VTKParser/depend/VTKParser.cpp  # Add the .cpp file, assuming it contains the implementation
        depend/argparse.hpp
)

# Include the directories with header files
target_include_directories(nex PRIVATE depend VTKParser/depend)


