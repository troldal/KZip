add_executable(ZippyTest "")
target_sources(ZippyTest
        PRIVATE
            ZippyTest.cpp)

target_link_libraries(ZippyTest PUBLIC Zippy)
target_include_directories(ZippyTest PRIVATE ${CMAKE_CURRENT_LIST_DIR}/catch2)