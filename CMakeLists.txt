cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Form_Up)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Form Up Game ####
file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_executable(FORMUP_PROJECT  ${SOURCES})
target_include_directories(FORMUP_PROJECT  PRIVATE ${SFML_INCS})
target_link_libraries(FORMUP_PROJECT  sfml-graphics)

# Set the working directory for Visual Studio's debugger to the output directory
set_target_properties(FORMUP_PROJECT 
    PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)

# Custom target to copy resources (like the 'res' folder) to the output directory
add_custom_target(copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/res"   # Source directory for resources
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res  # Destination directory
)

# Make FORMUP_PROJECT  depend on copy_resources so resources are copied before it runs
add_dependencies(FORMUP_PROJECT copy_resources)