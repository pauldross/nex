cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(nex)

find_package(VTK COMPONENTS 
  CommonCore
  CommonDataModel
  IOLegacy
  CommonColor
  FiltersCore
  FiltersSources
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  RenderingLabel
)

#include(FetchContent)
#FetchContent_Declare(
#        argparse
#        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
#)
#FetchContent_MakeAvailable(argparse)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "nex: Unable to find the VTK build folder.")
endif()

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
        depend/noiseEX_custom_parser.cpp
        depend/noiseEX_custom_parser.h
)
  target_link_libraries(nex PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS nex
  MODULES ${VTK_LIBRARIES}
)
