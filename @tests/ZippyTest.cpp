//
// Created by Troldal on 2019-03-13.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}


//test_load_file();
//test_load_stream();
//test_load_bytes();
//test_reset();
//test_getinfo();
//test_infolist();
//test_namelist();
//test_open_by_name();
//test_open_by_info();
//test_extract_current_directory();
//test_extract_path();
//test_extractall_current_directory();
//test_extractall_path();
//test_extractall_members_name();
//test_extractall_members_info();
//test_printdir();
//test_read();
//test_testzip();
//test_write();
//test_writestr();
//test_comment();

TEST_CASE( "Factorials are computed", "[factorial]" ) {
REQUIRE( Factorial(1) == 1 );
REQUIRE( Factorial(2) == 2 );
REQUIRE( Factorial(3) == 6 );
REQUIRE( Factorial(10) == 3628800 );
}