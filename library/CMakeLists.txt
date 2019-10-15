#=======================================================================================================================
# Define library targets
#=======================================================================================================================

# MINIZ library
# A separate library target is defined, in order to be able to suppress compiler warnings using SYSTEM INTERFACE specifiers
add_library(miniz INTERFACE)
target_include_directories(miniz SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR}/Zippy/miniz/)

# ZIPPY library
add_library(Zippy INTERFACE)
add_library(Zippy::Zippy ALIAS Zippy)
target_link_libraries(Zippy INTERFACE miniz)
target_include_directories(Zippy INTERFACE ${CMAKE_CURRENT_LIST_DIR})

#=======================================================================================================================
# Install Zippy Library
#=======================================================================================================================
#include(GNUInstallDirs)
#install(TARGETS Zippy
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
#install(FILES ${Zippy_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Zippy)