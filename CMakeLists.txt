#=======================================================================================================================
# Preamble
#=======================================================================================================================
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(SimpleZip C CXX)
#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
#set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

#=======================================================================================================================
# Set project metadata
#=======================================================================================================================
set(PROJECT_VENDOR "Kenneth Troldal Balslev")
set(PROJECT_CONTACT "kenneth.balslev@gmail.com")
set(PROJECT_URL "https://github.com/troldal/SimpleZip")
set(PROJECT_DESCRIPTION "A C++17 wrapper around the 'miniz' zip file library")

#=======================================================================================================================
# Set C/C++ compiler version
#=======================================================================================================================
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Consider moving these to specific targets and/or make them optional
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

#=======================================================================================================================
# Add project subdirectories
#=======================================================================================================================
add_subdirectory(@library)
add_subdirectory(@examples)
add_subdirectory(@tests)
add_subdirectory(@doxy)

#=======================================================================================================================
# Define internal dependencies
#=======================================================================================================================
add_dependencies(Example1 SimpleZip)
add_dependencies(SimpleZipTest SimpleZip)

