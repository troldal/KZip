#=======================================================================================================================
# Preamble
#=======================================================================================================================
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(Zippy C CXX)

#=======================================================================================================================
# Set project metadata
#=======================================================================================================================
set(PROJECT_VENDOR "Kenneth Troldal Balslev")
set(PROJECT_CONTACT "kenneth.balslev@gmail.com")
set(PROJECT_URL "https://github.com/troldal/Zippy")
set(PROJECT_DESCRIPTION "A C++17 wrapper around the 'miniz' zip file library")

#=======================================================================================================================
# Set C/C++ compiler version
#=======================================================================================================================
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#=======================================================================================================================
# Add build options
#=======================================================================================================================

# NOTE: Zippy is header-only, so without samples or the tests, there are not targets to be built.
option(CREATE_DOCS "Build library documentation (requires Doxygen and Graphviz/Dot to be installed)" ON)
option(BUILD_SAMPLES "Build sample programs" OFF)
option(BUILD_TESTS "Build and run library tests" ON)

# Consider moving these to specific targets and/or make them optional
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=leak")
#set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=leak")

#=======================================================================================================================
# Add project subdirectories
#=======================================================================================================================
add_subdirectory(library)

if(${CREATE_DOCS})
    add_subdirectory(doxy)
endif()

if(${BUILD_TESTS})
    add_subdirectory(tests)
endif()

if(${BUILD_SAMPLES})
    add_subdirectory(examples)
endif()
