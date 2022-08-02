#=======================================================================================================================
# Define Zippy KZip targets
#=======================================================================================================================
add_library(KZip INTERFACE)
add_library(KZip::KZip ALIAS KZip)
target_include_directories(KZip INTERFACE ${CMAKE_CURRENT_LIST_DIR})
