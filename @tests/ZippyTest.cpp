//
// Created by Troldal on 2019-03-13.
//

#include "catch.hpp"
#include <Zippy/Zippy.h>

TEST_CASE("Test 1: Create new archive") {

    REQUIRE(false);
}

TEST_CASE("Test 2: Open existing archive") {

    // Set up
    Zippy::ZipArchive archive;
    archive.Open("./CreatedWithWinZip.zip");

    SECTION("Count entries") {
        REQUIRE(archive.GetNumEntries() == 12);
    }

    SECTION("List entries") {
        REQUIRE(false);
    }

    SECTION("Count entries in directory") {
        REQUIRE(false);
    }

    SECTION("List entries in directory") {
        REQUIRE(false);
    }

    SECTION("Check entries exist") {
        REQUIRE(false);
    }

    SECTION("Extract entry to disk") {
        REQUIRE(false);
    }

    SECTION("Get entry binary data") {
        REQUIRE(false);
    }

    SECTION("Get entry string data") {
        REQUIRE(false);
    }

    SECTION("Get entry meta data") {
        REQUIRE(false);
    }
}

TEST_CASE("Test 3: Modify archive") {

    // Set up

    SECTION("Add entry") {
        REQUIRE(false);
    }

    SECTION("Delete entry") {
        REQUIRE(false);
    }

    SECTION("Set entry binary data") {
        REQUIRE(false);
    }

    SECTION("Set entry string data") {
        REQUIRE(false);
    }

    SECTION("Set entry comment data") {
        REQUIRE(false);
    }

    SECTION("Save archive") {
        REQUIRE(false);
    }

    SECTION("Save archive as") {
        REQUIRE(false);
    }
}

TEST_CASE("Test 4: Class operations") {

    // Set up

    SECTION("Archive move") {
        REQUIRE(false);
    }

    SECTION("Entry move") {
        REQUIRE(false);
    }

    SECTION("Entry copy") {
        REQUIRE(false);
    }
}