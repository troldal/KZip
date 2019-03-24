add_executable(ZippyTest "")
target_sources(ZippyTest
        PRIVATE
        TestMain.cpp
        ZippyTest.cpp)

target_include_directories(ZippyTest PRIVATE ${CMAKE_CURRENT_LIST_DIR}/catch2 ${ZIPPY_INSTALLDIR}/include)
#target_link_libraries(ZippyTest PUBLIC Zippy)
