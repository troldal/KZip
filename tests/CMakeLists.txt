#=======================================================================================================================
# Define CATCH library target
#=======================================================================================================================
add_library(Catch INTERFACE)
target_include_directories(Catch SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR}/catch2)

#=======================================================================================================================
# Define TEST target
#=======================================================================================================================
add_executable(ZippyTest "")
target_sources(ZippyTest
        PRIVATE
        TestMain.cpp
        ZippyTest.cpp)

target_link_libraries(ZippyTest PRIVATE Catch Zippy)

#=======================================================================================================================
# Set compiler flags
#=======================================================================================================================
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        #target_compile_options(ZippyTest PRIVATE -Wlifetime)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(ZippyTest PRIVATE -Wmisleading-indentation)
        target_compile_options(ZippyTest PRIVATE -Wduplicated-cond)
        target_compile_options(ZippyTest PRIVATE -Wduplicated-branches)
        target_compile_options(ZippyTest PRIVATE -Wlogical-op)
        target_compile_options(ZippyTest PRIVATE -Wnull-dereference)
        target_compile_options(ZippyTest PRIVATE -Wuseless-cast)
    elseif (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
        target_compile_options(ZippyTest PRIVATE -Wall)
        target_compile_options(ZippyTest PRIVATE -Wextra)
        target_compile_options(ZippyTest PRIVATE -Wshadow)
        target_compile_options(ZippyTest PRIVATE -Wnon-virtual-dtor)
        target_compile_options(ZippyTest PRIVATE -Wold-style-cast)
        target_compile_options(ZippyTest PRIVATE -Wcast-align)
        target_compile_options(ZippyTest PRIVATE -Wunused)
        target_compile_options(ZippyTest PRIVATE -Woverloaded-virtual)
        target_compile_options(ZippyTest PRIVATE -Wpedantic)
        target_compile_options(ZippyTest PRIVATE -Wconversion)
        target_compile_options(ZippyTest PRIVATE -Wsign-conversion)
        target_compile_options(ZippyTest PRIVATE -Wdouble-promotion)
        target_compile_options(ZippyTest PRIVATE -Wformat=2)
        target_compile_options(ZippyTest PRIVATE -Weffc++)
    elseif (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"))
        target_compile_options(ZippyTest PRIVATE /permissive)
        target_compile_options(ZippyTest PRIVATE /W4)
        target_compile_options(ZippyTest PRIVATE /w14242)
        target_compile_options(ZippyTest PRIVATE /w14254)
        target_compile_options(ZippyTest PRIVATE /w14263)
        target_compile_options(ZippyTest PRIVATE /w14265)
        target_compile_options(ZippyTest PRIVATE /w14287)
        target_compile_options(ZippyTest PRIVATE /we4289)
        target_compile_options(ZippyTest PRIVATE /w14296)
        target_compile_options(ZippyTest PRIVATE /w14311)
        target_compile_options(ZippyTest PRIVATE /w14545)
        target_compile_options(ZippyTest PRIVATE /w14546)
        target_compile_options(ZippyTest PRIVATE /w14547)
        target_compile_options(ZippyTest PRIVATE /w14549)
        target_compile_options(ZippyTest PRIVATE /w14555)
        target_compile_options(ZippyTest PRIVATE /w14619)
        target_compile_options(ZippyTest PRIVATE /w14640)
        target_compile_options(ZippyTest PRIVATE /w14826)
        target_compile_options(ZippyTest PRIVATE /w14905)
        target_compile_options(ZippyTest PRIVATE /w14906)
        target_compile_options(ZippyTest PRIVATE /w14928)
    endif ()
endif ()

# Consider making them optional
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=leak")
#set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=leak")