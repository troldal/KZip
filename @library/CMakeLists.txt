#=======================================================================================================================
# Define library targets
#=======================================================================================================================
add_library(Zippy STATIC "")
add_library(Zippy::Zippy ALIAS Zippy)

#=======================================================================================================================
# Set compiler flags
#=======================================================================================================================
include(CheckIPOSupported)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(Zippy PRIVATE -Wall)
    target_compile_options(Zippy PRIVATE -Wextra)
    target_compile_options(Zippy PRIVATE -pedantic)
    #target_compile_options(Zippy PRIVATE -fsanitize=leak -g)
    #target_link_options(Zippy PRIVATE -fsanitize=leak -g)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        #target_compile_options(Zippy PRIVATE -Weverything)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(Zippy PRIVATE -Weffc++)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        # using Intel C++
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        # using Visual Studio C++
    endif ()
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    check_ipo_supported(RESULT result OUTPUT output)
    if (result)
        set_property(TARGET Zippy PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif ()
endif ()

#=======================================================================================================================
# Find project files
#=======================================================================================================================
set(Zippy_IMPL_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/ZipArchive.h
        ${CMAKE_CURRENT_LIST_DIR}/ZipEntry.h
        ${CMAKE_CURRENT_LIST_DIR}/ZipException.h
        ${CMAKE_CURRENT_LIST_DIR}/miniz/miniz.h)

set(Zippy_IMPL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/ZipArchive.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ZipEntry.cpp
        ${CMAKE_CURRENT_LIST_DIR}/miniz/miniz.c)

#=======================================================================================================================
# Build Zippy library
#=======================================================================================================================
#include(GenerateExportHeader)
#generate_export_header(Zippy)
target_sources(Zippy
        PRIVATE
            ${Zippy_IMPL_SOURCES})

# ===== Copy header files to (staged) install directory ===== #
file(REMOVE_RECURSE ${Zippy_INSTALLDIR}/include/)
file(COPY
        ${CMAKE_CURRENT_LIST_DIR}/ZipArchive.h
        ${CMAKE_CURRENT_LIST_DIR}/ZipEntry.h
        ${CMAKE_CURRENT_LIST_DIR}/ZipException.h
        ${CMAKE_CURRENT_LIST_DIR}/Zippy.h
     DESTINATION
        ${Zippy_INSTALLDIR}/include/Zippy/)

file(COPY
        ${CMAKE_CURRENT_LIST_DIR}/miniz/miniz.h
     DESTINATION
        ${Zippy_INSTALLDIR}/include/Zippy/miniz)

#=======================================================================================================================
# Install Zippy Library
#=======================================================================================================================
#include(GNUInstallDirs)
#install(TARGETS OpenXLSX
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
#install(FILES ${OPENXLSX_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/OpenXLSX)