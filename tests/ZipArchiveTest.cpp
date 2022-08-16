//
// Created by Troldal on 2019-03-13.
//

#include "test-data-binary.hpp"
#include "test-data-text.hpp"
#include <KZip.hpp>
#include <catch.hpp>

// Add binary file to archive
// Add folders to archive
// Open existing archive
// List contents in archive

TEST_CASE("TEST 1: CREATE ARCHIVE") {

        /**
         * @brief Test that trying to do operations on an empty archive object will throw an exception.
         */
    SECTION("#01: Creation of empty (invalid) ZipArchive object.") {
        KZip::ZipArchive archive;
        REQUIRE(!archive.isOpen());
        REQUIRE_THROWS(archive.entryNames());
        REQUIRE_THROWS(archive.entryNames(""));
        REQUIRE_THROWS(archive.entryCount());
        REQUIRE_THROWS(archive.entryCount(""));
        REQUIRE_THROWS(archive.hasEntry(""));
        REQUIRE_THROWS(archive.save());
        REQUIRE_THROWS(archive.deleteEntry(""));
        REQUIRE_THROWS(archive.entry(""));
//        REQUIRE_THROWS(archive.ExtractEntry("", ""));
//        REQUIRE_THROWS(archive.ExtractDir("", ""));
//        REQUIRE_THROWS(archive.ExtractAll(""));
//        REQUIRE_THROWS(archive.AddEntry("Entry.txt", "TEST"));
    }

        /**
         * @brief
         */
    SECTION("#02: Create empty archive file") {
        KZip::ZipArchive archive;
        archive.create("./TestArchive.zip");
        REQUIRE(archive.isOpen());
        REQUIRE(archive.entryNames().empty());
        REQUIRE(archive.entryNames("").empty());
        REQUIRE(archive.entryCount() == 0);
        REQUIRE(archive.entryCount("") == 0);
    }
}

TEST_CASE("ZipEntry") {

    std::string entryData = txtdata;

    KZip::ZipEntry first("TestEntry");
    REQUIRE(first.name() == "TestEntry");

    first = entryData;
    REQUIRE(first == entryData);
    REQUIRE_FALSE(first == std::string("dummy string"));

    auto second = first;
    REQUIRE(second.name() == "TestEntry");
    REQUIRE(second == entryData);
    REQUIRE_FALSE(second == std::string("dummy string"));

    KZip::ZipEntry third("dummy");
    third = first;
    REQUIRE(third.name() == "TestEntry");
    REQUIRE(third == entryData);
    REQUIRE_FALSE(third == std::string("dummy string"));

    std::string data = third;

}

TEST_CASE("TEST 2: ADD & EXTRACT ENTRIES") {

    KZip::ZipArchive archive;
    std::string archivePath = "./TestArchive.zip";
    archive.create(archivePath);

    /**
     * @brief
     */
    SECTION("#01: Add text data to archive") {
        std::string entryName = "text_data.txt";
        std::string entryData = txtdata;

        archive.addEntry(entryName) = entryData;
        REQUIRE(archive.isOpen());
        REQUIRE(archive.entryNames().size() == 1);
        REQUIRE(archive.entryNames().at(0) == entryName);
        REQUIRE(archive.entryNames("").size() == 1);
        REQUIRE(archive.entryNames("").at(0) == entryName);
        REQUIRE(archive.entryCount() == 1);
        REQUIRE(archive.entryCount("") == 1);
        REQUIRE(archive.hasEntry(entryName));

        // ===== Test using ZipEntryProxy
        REQUIRE(archive.entry(entryName) == entryData);
        REQUIRE_FALSE(archive.entry(entryName) == std::string("dummy string"));
        REQUIRE(archive.entry(entryName).name() == entryName);

        // ===== Test using ZipEntry object
        KZip::ZipEntry entry = archive.entry(entryName);
        REQUIRE(entry == entryData);
        REQUIRE_FALSE(entry == std::string("dummy string"));
        REQUIRE(entry.name() == entryName);

        archive.save();
        archive.close();
        REQUIRE_FALSE(archive.isOpen());

        archive.open(archivePath);
        REQUIRE(archive.isOpen());
        REQUIRE(archive.entryNames().size() == 1);
        REQUIRE(archive.entryNames().at(0) == entryName);
        REQUIRE(archive.entryNames("").size() == 1);
        REQUIRE(archive.entryNames("").at(0) == entryName);
        REQUIRE(archive.entryCount() == 1);
        REQUIRE(archive.entryCount("") == 1);
        REQUIRE(archive.hasEntry(entryName));
        REQUIRE(archive.entry(entryName) == entryData);
        REQUIRE_FALSE(archive.entry(entryName) == std::string("dummy string"));
    }

    /**
     * @brief
     */
    SECTION("Add binary data to archive") {
        std::string entryName = "binary_data.png";
        auto entryData = bindata;

        archive.create(archivePath);
        archive.addEntry(entryName) = entryData;
        REQUIRE(archive.isOpen());
        REQUIRE(archive.entryNames().size() == 1);
        REQUIRE(archive.entryNames().at(0) == entryName);
        REQUIRE(archive.entryNames("").size() == 1);
        REQUIRE(archive.entryNames("").at(0) == entryName);
        REQUIRE(archive.entryCount() == 1);
        REQUIRE(archive.entryCount("") == 1);
        REQUIRE(archive.hasEntry(entryName));
        REQUIRE(archive.entry(entryName) == entryData);

        archive.save();
        archive.close();
        REQUIRE_FALSE(archive.isOpen());

        archive.open(archivePath);
        REQUIRE(archive.isOpen());
        REQUIRE(archive.entryNames().size() == 1);
        REQUIRE(archive.entryNames().at(0) == entryName);
        REQUIRE(archive.entryNames("").size() == 1);
        REQUIRE(archive.entryNames("").at(0) == entryName);
        REQUIRE(archive.entryCount() == 1);
        REQUIRE(archive.entryCount("") == 1);
        REQUIRE(archive.hasEntry(entryName));
        REQUIRE(archive.entry(entryName) == entryData);
    }
}

TEST_CASE("TEST 3: OPEN EXISTING ARCHIVE") {

    // Set up
    KZip::ZipArchive archive;
    archive.open("./CreatedWithWinZip.zip");

    SECTION("Count entries") {
        REQUIRE(archive.entryCount(KZip::ZipFlags::Files) == 10);
        REQUIRE(archive.entryCount(KZip::ZipFlags::Directories) == 2);
        REQUIRE(archive.entryCount(KZip::ZipFlags::Files | KZip::ZipFlags::Directories) == 12);
    }

    SECTION("List files") {
        auto names = archive.entryNames(KZip::ZipFlags::Files);
        REQUIRE(std::find(names.begin(), names.end(), "file 1.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "file 2.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "file 3.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "file 4.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "file 5.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 1.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 2.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 3.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 4.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 5.txt") != names.end());
    }

    SECTION("List folders") {
        auto names = archive.entryNames(KZip::ZipFlags::Directories);
        REQUIRE(std::find(names.begin(), names.end(), "Folder 1/") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/") != names.end());
    }

    SECTION("List all") {
        auto names = archive.entryNames(KZip::ZipFlags::Files | KZip::ZipFlags::Directories);
        REQUIRE(std::find(names.begin(), names.end(), "file 1.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "file 2.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "file 3.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "file 4.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "file 5.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 1/") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 1.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 2.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 3.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 4.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 5.txt") != names.end());
    }

    SECTION("Count entries in directory") {
        REQUIRE(archive.entryCount("Folder 2/") == 5);
    }

    SECTION("List entries in directory") {
        auto names = archive.entryNames("Folder 2/");

        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 1.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 2.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 3.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 4.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 5.txt") != names.end());
    }

    SECTION("Check entries exist") {
        REQUIRE(archive.hasEntry("file 1.txt"));
        REQUIRE(archive.hasEntry("file 2.txt"));
        REQUIRE(archive.hasEntry("file 3.txt"));
        REQUIRE(archive.hasEntry("file 4.txt"));
        REQUIRE(archive.hasEntry("file 5.txt"));
        REQUIRE(archive.hasEntry("Folder 1/"));
        REQUIRE(archive.hasEntry("Folder 2/"));
        REQUIRE(archive.hasEntry("Folder 2/file 1.txt"));
        REQUIRE(archive.hasEntry("Folder 2/file 2.txt"));
        REQUIRE(archive.hasEntry("Folder 2/file 3.txt"));
        REQUIRE(archive.hasEntry("Folder 2/file 4.txt"));
        REQUIRE(archive.hasEntry("Folder 2/file 5.txt"));
    }

//    SECTION("Extract entry to disk") {
//        REQUIRE(false);
//    }
//
//    SECTION("Extract directory to disk") {
//        REQUIRE(false);
//    }
//
//    SECTION("Extract all to disk") {
//        REQUIRE(false);
//    }
//
//    SECTION("Get entry binary data") {
//        auto result = archive.GetEntry("file 1.txt").GetData();
//        std::string resultStr;
//        for (auto& ch : result)
//            resultStr += static_cast<char>(ch);
//
//        std::string compare = "MISSION CONTROL I wouldn't worry too much about the computer. First of all, there is still a chance that he is right, despite your tests, and if it should happen again, we suggest eliminating this possibility by allowing the unit to remain in place and seeing whether or not it actually fails. If the computer should turn out to be wrong, the situation is still not alarming. The type of obsessional error he may be guilty of is not unknown among the latest generation of HAL 9000 computers. It has almost always revolved around a single detail, such as the one you have described, and it has never interfered with the integrity or reliability of the computer's performance in other areas. No one is certain of the cause of this kind of malfunctioning. It may be over-programming, but it could also be any number of reasons. In any event, it is somewhat analogous to human neurotic behavior. Does this answer your query?  Zero-five-three-Zero, MC, transmission concluded.";
//        REQUIRE(resultStr == compare);
//    }
//
//    SECTION("Get entry string data") {
//        std::string result = archive.GetEntry("file 1.txt").GetDataAsString();
//        std::string compare = "MISSION CONTROL I wouldn't worry too much about the computer. First of all, there is still a chance that he is right, despite your tests, and if it should happen again, we suggest eliminating this possibility by allowing the unit to remain in place and seeing whether or not it actually fails. If the computer should turn out to be wrong, the situation is still not alarming. The type of obsessional error he may be guilty of is not unknown among the latest generation of HAL 9000 computers. It has almost always revolved around a single detail, such as the one you have described, and it has never interfered with the integrity or reliability of the computer's performance in other areas. No one is certain of the cause of this kind of malfunctioning. It may be over-programming, but it could also be any number of reasons. In any event, it is somewhat analogous to human neurotic behavior. Does this answer your query?  Zero-five-three-Zero, MC, transmission concluded.";
//        REQUIRE(result == compare);
//    }
//
//    SECTION("Get entry meta data") {
//
//
//        REQUIRE(false);
//    }
}
//
//TEST_CASE("Test 3: Modify archive") {
//
//    // Set up
//
//    SECTION("Add entry") {
//        REQUIRE(false);
//    }
//
//    SECTION("Delete entry") {
//        REQUIRE(false);
//    }
//
//    SECTION("Set entry binary data") {
//        REQUIRE(false);
//    }
//
//    SECTION("Set entry string data") {
//        REQUIRE(false);
//    }
//
//    SECTION("Set entry comment data") {
//        REQUIRE(false);
//    }
//
//    SECTION("Save archive") {
//        REQUIRE(false);
//    }
//
//    SECTION("Save archive as") {
//        REQUIRE(false);
//    }
//}
//
//TEST_CASE("Test 4: Class operations") {
//
//    // Set up
//
//    SECTION("Archive move") {
//        REQUIRE(false);
//    }
//
//    SECTION("Entry move") {
//        REQUIRE(false);
//    }
//
//    SECTION("Entry copy") {
//        REQUIRE(false);
//    }
//}