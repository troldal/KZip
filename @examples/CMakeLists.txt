#=======================================================================================================================
# Define Example1 target
#=======================================================================================================================
add_executable(Example1 Example1.cpp)
target_include_directories(Example1 PUBLIC ${SIMPLEZIP_INSTALLDIR}/include)
target_link_libraries(Example1 PUBLIC SimpleZip)