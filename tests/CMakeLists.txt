#=======================================================================================================================
# Preamble
#=======================================================================================================================
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(KZip.Tests)

#=======================================================================================================================
# Set up Catch2 library
#=======================================================================================================================
add_library(Catch INTERFACE IMPORTED)
target_include_directories(Catch SYSTEM INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/catch/>)

#=======================================================================================================================
# Define TEST target
#=======================================================================================================================
add_executable(KZipTests "")
target_sources(KZipTests
        PRIVATE
        TestMain.cpp
        ZipArchiveTest.cpp)

target_link_libraries(KZipTests PRIVATE Catch KZip)

#=======================================================================================================================
# Set compiler flags
#=======================================================================================================================
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        #target_compile_options(ZippyTest PRIVATE -Wlifetime)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(KZipTests PRIVATE -Wmisleading-indentation)
        target_compile_options(KZipTests PRIVATE -Wduplicated-cond)
        target_compile_options(KZipTests PRIVATE -Wduplicated-branches)
        target_compile_options(KZipTests PRIVATE -Wlogical-op)
        target_compile_options(KZipTests PRIVATE -Wnull-dereference)
        target_compile_options(KZipTests PRIVATE -Wuseless-cast)
    elseif (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
        target_compile_options(KZipTests PRIVATE -Wall)
        target_compile_options(KZipTests PRIVATE -Wextra)
        target_compile_options(KZipTests PRIVATE -Wshadow)
        target_compile_options(KZipTests PRIVATE -Wnon-virtual-dtor)
        target_compile_options(KZipTests PRIVATE -Wold-style-cast)
        target_compile_options(KZipTests PRIVATE -Wcast-align)
        target_compile_options(KZipTests PRIVATE -Wunused)
        target_compile_options(KZipTests PRIVATE -Woverloaded-virtual)
        target_compile_options(KZipTests PRIVATE -Wpedantic)
        target_compile_options(KZipTests PRIVATE -Wconversion)
        target_compile_options(KZipTests PRIVATE -Wsign-conversion)
        target_compile_options(KZipTests PRIVATE -Wdouble-promotion)
        target_compile_options(KZipTests PRIVATE -Wformat=2)
        target_compile_options(KZipTests PRIVATE -Weffc++)
    elseif (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"))
        target_compile_options(KZipTests PRIVATE /permissive)
        target_compile_options(KZipTests PRIVATE /W4)
        target_compile_options(KZipTests PRIVATE /w14242)
        target_compile_options(KZipTests PRIVATE /w14254)
        target_compile_options(KZipTests PRIVATE /w14263)
        target_compile_options(KZipTests PRIVATE /w14265)
        target_compile_options(KZipTests PRIVATE /w14287)
        target_compile_options(KZipTests PRIVATE /we4289)
        target_compile_options(KZipTests PRIVATE /w14296)
        target_compile_options(KZipTests PRIVATE /w14311)
        target_compile_options(KZipTests PRIVATE /w14545)
        target_compile_options(KZipTests PRIVATE /w14546)
        target_compile_options(KZipTests PRIVATE /w14547)
        target_compile_options(KZipTests PRIVATE /w14549)
        target_compile_options(KZipTests PRIVATE /w14555)
        target_compile_options(KZipTests PRIVATE /w14619)
        target_compile_options(KZipTests PRIVATE /w14640)
        target_compile_options(KZipTests PRIVATE /w14826)
        target_compile_options(KZipTests PRIVATE /w14905)
        target_compile_options(KZipTests PRIVATE /w14906)
        target_compile_options(KZipTests PRIVATE /w14928)
    endif ()
endif ()

#=======================================================================================================================
# Enable code coverage
#=======================================================================================================================
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT WIN32)
#    target_compile_options(KZipTests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
#    target_link_options(KZipTests PRIVATE -fprofile-instr-generate)
#    #Uncomment in case of linker errors
#    #link_libraries(clang_rt.profile-x86_64)
#elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#    target_compile_options(KZipTests PRIVATE --coverage)
#    target_link_options(KZipTests PRIVATE --coverage)
#    #Uncomment in case of linker errors
#    #link_libraries(gcov)
#endif ()

# Consider making them optional
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=leak")
#set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=leak")