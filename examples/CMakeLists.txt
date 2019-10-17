#=======================================================================================================================
# Define Example1 target
#=======================================================================================================================
add_executable(Example1 example1.cpp)
target_include_directories(Example1 PUBLIC ${ZIPPY_INSTALLDIR}/include)
#target_link_libraries(Example1 PUBLIC Zippy)