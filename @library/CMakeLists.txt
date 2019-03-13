#=======================================================================================================================
# Define library targets
#=======================================================================================================================
add_library(SimpleZip STATIC "")
add_library(SimpleZip::SimpleZip ALIAS SimpleZip)

#=======================================================================================================================
# Set compiler flags
#=======================================================================================================================
include(CheckIPOSupported)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(SimpleZip PRIVATE -Wall)
    target_compile_options(SimpleZip PRIVATE -Wextra)
    target_compile_options(SimpleZip PRIVATE -pedantic)
    #target_compile_options(SimpleZip PRIVATE -fsanitize=leak -g)
    #target_link_options(SimpleZip PRIVATE -fsanitize=leak -g)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        #target_compile_options(SimpleZip PRIVATE -Weverything)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(SimpleZip PRIVATE -Weffc++)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        # using Intel C++
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # using Visual Studio C++
    endif ()
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    check_ipo_supported(RESULT result OUTPUT output)
    if (result)
        set_property(TARGET SimpleZip PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif ()
endif ()

#=======================================================================================================================
# Find project files
#=======================================================================================================================
set(SIMPLEZIP_IMPL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/ZipArchive.h
        ${CMAKE_CURRENT_LIST_DIR}/ZipEntry.h
        ${CMAKE_CURRENT_LIST_DIR}/ZipException.h
        ${CMAKE_CURRENT_LIST_DIR}/miniz/miniz.h)

set(SIMPLEZIP_IMPL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/ZipArchive.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ZipEntry.cpp
        ${CMAKE_CURRENT_LIST_DIR}/miniz/miniz.c)

#=======================================================================================================================
# Build OpenXLSX library
#=======================================================================================================================
include(GenerateExportHeader)
generate_export_header(SimpleZip)
target_sources(SimpleZip
        PRIVATE
        ${SIMPLEZIP_IMPL_SOURCES})

#=======================================================================================================================
# Install OpenXLSX Library
#=======================================================================================================================
#include(GNUInstallDirs)
#install(TARGETS OpenXLSX
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
#install(FILES ${OPENXLSX_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/OpenXLSX)