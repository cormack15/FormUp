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
target_link_libraries(FORMUP_PROJECT  sfml-graphics sfml-audio)

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

#INSTALLER CODE

#Install directory within build folder, so it doesn't go into /lib or program-files
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/")

#Installer.exe/zip built into /installer
SET(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/installer/")

#Install component for runtime dlls and stuff
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE) #stop this happening on non-release builds
INCLUDE(InstallRequiredSystemLibraries)
install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "bin/" CONFIGURATIONS Release COMPONENT SYSTEM_CMPT)

#Install component for resources
install(DIRECTORY "${PROJECT_SOURCE_DIR}/res" DESTINATION "bin" CONFIGURATIONS Release COMPONENT RESOURCES_CMPT)

#We disabled SFML's INSTALL component as it brings along junk, so we gotta make one ourselves
install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/" DESTINATION "bin" CONFIGURATIONS Release COMPONENT SFML_CMPT FILES_MATCHING PATTERN "*.dll" )

#Main Install target
install(TARGETS FORMUP_PROJECT DESTINATION "bin" CONFIGURATIONS Release COMPONENT FORMUP_PROJECT_CMPT)


#Tell CPack about the libs and resources install components
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} RESOURCES_CMPT SFML_CMPT SYSTEM_CMPT FORMUP_PROJECT_CMPT)
set(CPACK_COMPONENT_FORMUP_PROJECT_CMPT_DEPENDS RESOURCES_CMPT SFML_CMPT SYSTEM_CMPT)
set(CPACK_COMPONENT_RESOURCES_CMPT_REQUIRED TRUE)
set(CPACK_COMPONENT_SFML_CMPT_REQUIRED TRUE)
set(CPACK_COMPONENT_SYSTEM_CMPT_REQUIRED TRUE)
set(CPACK_COMPONENT_FORMUP_PROJECT_CMPT_DISPLAY_NAME "Form Up")


IF(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_NSIS_COMPONENT_INSTALL ON)
  set(CPACK_NSIS_HELP_LINK "https://github.com/cormack15/FormUp")
  set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/cormack15/FormUp")
  set(CPACK_NSIS_CONTACT "https://github.com/cormack15")
  	#Commented out icon image part, can add later if needed
	#set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/res/img/icon.ico")
ELSE()
  set(CPACK_GENERATOR ZIP)
ENDIF(WIN32)


set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Form_Up")
set(CPACK_PACKAGE_VENDOR "FormUp Group")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")

INCLUDE(CPack)