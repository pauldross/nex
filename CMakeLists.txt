cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(nex)



# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
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
        VTKParser/depend/VTKParser.h
        VTKParser/depend/VTKParser.cpp
)
#  target_link_libraries(nex PRIVATE ${VTK_LIBRARIES}
#)
# vtk_module_autoinit is needed


