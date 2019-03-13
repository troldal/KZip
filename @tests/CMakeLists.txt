add_executable(SimpleZipTest "")
target_sources(SimpleZipTest
        PRIVATE
            SimpleZipTest.cpp)

target_link_libraries(SimpleZipTest PUBLIC SimpleZip)
target_include_directories(SimpleZipTest PRIVATE ${CMAKE_CURRENT_LIST_DIR}/catch2)