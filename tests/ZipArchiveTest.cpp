//
// Created by Troldal on 2019-03-13.
//

#include "test-data-binary.hpp"
#include "test-data-text.hpp"
#include <KZip.hpp>
#include <catch.hpp>
#include <deque>

// Add binary file to archive
// Add folders to archive
// Open existing archive
// List contents in archive

TEST_CASE("TEST 1: Create Archive") {

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

TEST_CASE("TEST 2: ZipEntry Created Manually") {

    std::string entryData = txtdata;

    // ===== Tests for ensuring that a default constructed ZipEntry file object works as expected.
    SECTION("#01: Default Construction") {

        // ===== Create ZipEntry and add data.
        KZip::ZipEntry entry("text_data.txt");
        entry = entryData;

        // ===== Check that name and contents are correct
        REQUIRE(entry.name() == "text_data.txt");
        REQUIRE(entry == entryData);
        REQUIRE_FALSE(entry == std::string("dummy string"));

        // ===== Change the entry name and check it is correct. Also check that an empty title is not allowed.
        entry.setName("other_text_data.txt");
        REQUIRE(entry.name() == "other_text_data.txt");
        REQUIRE_THROWS(entry.setName(""));

        // ===== Check that implicit conversion to std::string works as expected.
        std::string stringdata {};
        stringdata = entry;
        REQUIRE(stringdata == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::vector<unsigned char> vectordata {};
        vectordata = entry;
        REQUIRE(std::string{vectordata.begin(), vectordata.end()} == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::deque<unsigned char> dequedata {};
        dequedata = entry;
        REQUIRE(std::string{dequedata.begin(), dequedata.end()} == entryData);

        // ===== Check that data can be added in form of std::vector<unsigned char>
        vectordata = {entryData.begin(), entryData.end()};
        entry = vectordata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that data can be added in form of std::deque<unsigned char>
        dequedata = {entryData.begin(), entryData.end()};
        entry = dequedata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that the ZipEntry metadata are correct
        auto mdata = entry.metadata();
        REQUIRE(mdata.index() == 0);
        REQUIRE(mdata.compressedSize() == 0);
        REQUIRE(mdata.uncompressedSize() == entryData.size());
        REQUIRE(mdata.isDirectory() == false);
        REQUIRE(mdata.isEncrypted() == false);
        REQUIRE(mdata.isSupported() == true);
        REQUIRE(mdata.name() == "other_text_data.txt");
        REQUIRE(mdata.time() > 0);
    }

    // ===== Tests for ensuring that a copy constructed ZipEntry file object works as expected.
    SECTION("#02: Copy Construction") {

        // ===== Create ZipEntry, add data, and make a copy.
        KZip::ZipEntry original("text_data.txt");
        original = entryData;
        auto entry = original;

        // ===== Check that name and contents are correct
        REQUIRE(entry.name() == "text_data.txt");
        REQUIRE(entry == entryData);
        REQUIRE_FALSE(entry == std::string("dummy string"));

        // ===== Change the entry name and check it is correct. Also check that an empty title is not allowed.
        entry.setName("other_text_data.txt");
        REQUIRE(entry.name() == "other_text_data.txt");
        REQUIRE_THROWS(entry.setName(""));

        // ===== Check that implicit conversion to std::string works as expected.
        std::string stringdata {};
        stringdata = entry;
        REQUIRE(stringdata == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::vector<unsigned char> vectordata {};
        vectordata = entry;
        REQUIRE(std::string{vectordata.begin(), vectordata.end()} == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::deque<unsigned char> dequedata {};
        dequedata = entry;
        REQUIRE(std::string{dequedata.begin(), dequedata.end()} == entryData);

        // ===== Check that data can be added in form of std::vector<unsigned char>
        vectordata = {entryData.begin(), entryData.end()};
        entry = vectordata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that data can be added in form of std::deque<unsigned char>
        dequedata = {entryData.begin(), entryData.end()};
        entry = dequedata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that the ZipEntry metadata are correct
        auto mdata = entry.metadata();
        REQUIRE(mdata.index() == 0);
        REQUIRE(mdata.compressedSize() == 0);
        REQUIRE(mdata.uncompressedSize() == entryData.size());
        REQUIRE(mdata.isDirectory() == false);
        REQUIRE(mdata.isEncrypted() == false);
        REQUIRE(mdata.isSupported() == true);
        REQUIRE(mdata.name() == "other_text_data.txt");
        REQUIRE(mdata.time() > 0);
    }

    // ===== Tests for ensuring that a copy assigned ZipEntry file object works as expected.
    SECTION("#03: Copy Assignment") {

        // ===== Create ZipEntry, add data, and make a copy.
        KZip::ZipEntry original("text_data.txt");
        original = entryData;
        KZip::ZipEntry entry("dummy name");
        entry = original;

        // ===== Check that name and contents are correct
        REQUIRE(entry.name() == "text_data.txt");
        REQUIRE(entry == entryData);
        REQUIRE_FALSE(entry == std::string("dummy string"));

        // ===== Change the entry name and check it is correct. Also check that an empty title is not allowed.
        entry.setName("other_text_data.txt");
        REQUIRE(entry.name() == "other_text_data.txt");
        REQUIRE_THROWS(entry.setName(""));

        // ===== Check that implicit conversion to std::string works as expected.
        std::string stringdata {};
        stringdata = entry;
        REQUIRE(stringdata == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::vector<unsigned char> vectordata {};
        vectordata = entry;
        REQUIRE(std::string{vectordata.begin(), vectordata.end()} == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::deque<unsigned char> dequedata {};
        dequedata = entry;
        REQUIRE(std::string{dequedata.begin(), dequedata.end()} == entryData);

        // ===== Check that data can be added in form of std::vector<unsigned char>
        vectordata = {entryData.begin(), entryData.end()};
        entry = vectordata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that data can be added in form of std::deque<unsigned char>
        dequedata = {entryData.begin(), entryData.end()};
        entry = dequedata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that the ZipEntry metadata are correct
        auto mdata = entry.metadata();
        REQUIRE(mdata.index() == 0);
        REQUIRE(mdata.compressedSize() == 0);
        REQUIRE(mdata.uncompressedSize() == entryData.size());
        REQUIRE(mdata.isDirectory() == false);
        REQUIRE(mdata.isEncrypted() == false);
        REQUIRE(mdata.isSupported() == true);
        REQUIRE(mdata.name() == "other_text_data.txt");
        REQUIRE(mdata.time() > 0);
    }

    // ===== Tests for ensuring that a move constructed ZipEntry file object works as expected.
    SECTION("#04: Move Construction") {

        // ===== Create ZipEntry, add data, and make a copy.
        KZip::ZipEntry original("text_data.txt");
        original = entryData;
        auto entry = std::move(original);

        // ===== Check that calling finctions on the original object does not throw.
        REQUIRE_NOTHROW(original.name());
        REQUIRE_NOTHROW(original.data<std::string>());
        REQUIRE_NOTHROW(original.metadata());
        REQUIRE_NOTHROW(original.metadata().index());
        REQUIRE_NOTHROW(original.metadata().compressedSize());
        REQUIRE_NOTHROW(original.metadata().uncompressedSize());
        REQUIRE_NOTHROW(original.metadata().isDirectory());
        REQUIRE_NOTHROW(original.metadata().isEncrypted());
        REQUIRE_NOTHROW(original.metadata().isSupported());
        REQUIRE_NOTHROW(original.metadata().name());
        REQUIRE_NOTHROW(original.metadata().time());

        // ===== Check that name and contents are correct
        REQUIRE(entry.name() == "text_data.txt");
        REQUIRE(entry == entryData);
        REQUIRE_FALSE(entry == std::string("dummy string"));

        // ===== Change the entry name and check it is correct. Also check that an empty title is not allowed.
        entry.setName("other_text_data.txt");
        REQUIRE(entry.name() == "other_text_data.txt");
        REQUIRE_THROWS(entry.setName(""));

        // ===== Check that implicit conversion to std::string works as expected.
        std::string stringdata {};
        stringdata = entry;
        REQUIRE(stringdata == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::vector<unsigned char> vectordata {};
        vectordata = entry;
        REQUIRE(std::string{vectordata.begin(), vectordata.end()} == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::deque<unsigned char> dequedata {};
        dequedata = entry;
        REQUIRE(std::string{dequedata.begin(), dequedata.end()} == entryData);

        // ===== Check that data can be added in form of std::vector<unsigned char>
        vectordata = {entryData.begin(), entryData.end()};
        entry = vectordata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that data can be added in form of std::deque<unsigned char>
        dequedata = {entryData.begin(), entryData.end()};
        entry = dequedata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that the ZipEntry metadata are correct
        auto mdata = entry.metadata();
        REQUIRE(mdata.index() == 0);
        REQUIRE(mdata.compressedSize() == 0);
        REQUIRE(mdata.uncompressedSize() == entryData.size());
        REQUIRE(mdata.isDirectory() == false);
        REQUIRE(mdata.isEncrypted() == false);
        REQUIRE(mdata.isSupported() == true);
        REQUIRE(mdata.name() == "other_text_data.txt");
        REQUIRE(mdata.time() > 0);
    }

    // ===== Tests for ensuring that a move assigned ZipEntry file object works as expected.
    SECTION("#05: Move Assignment") {

        // ===== Create ZipEntry, add data, and make a copy.
        KZip::ZipEntry original("text_data.txt");
        original = entryData;
        KZip::ZipEntry entry("dummy name");
        entry = std::move(original);

        // ===== Check that calling finctions on the original object does not throw.
        REQUIRE_NOTHROW(original.name());
        REQUIRE_NOTHROW(original.data<std::string>());
        REQUIRE_NOTHROW(original.metadata());
        REQUIRE_NOTHROW(original.metadata().index());
        REQUIRE_NOTHROW(original.metadata().compressedSize());
        REQUIRE_NOTHROW(original.metadata().uncompressedSize());
        REQUIRE_NOTHROW(original.metadata().isDirectory());
        REQUIRE_NOTHROW(original.metadata().isEncrypted());
        REQUIRE_NOTHROW(original.metadata().isSupported());
        REQUIRE_NOTHROW(original.metadata().name());
        REQUIRE_NOTHROW(original.metadata().time());

        // ===== Check that name and contents are correct
        REQUIRE(entry.name() == "text_data.txt");
        REQUIRE(entry == entryData);
        REQUIRE_FALSE(entry == std::string("dummy string"));

        // ===== Change the entry name and check it is correct. Also check that an empty title is not allowed.
        entry.setName("other_text_data.txt");
        REQUIRE(entry.name() == "other_text_data.txt");
        REQUIRE_THROWS(entry.setName(""));

        // ===== Check that implicit conversion to std::string works as expected.
        std::string stringdata {};
        stringdata = entry;
        REQUIRE(stringdata == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::vector<unsigned char> vectordata {};
        vectordata = entry;
        REQUIRE(std::string{vectordata.begin(), vectordata.end()} == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::deque<unsigned char> dequedata {};
        dequedata = entry;
        REQUIRE(std::string{dequedata.begin(), dequedata.end()} == entryData);

        // ===== Check that data can be added in form of std::vector<unsigned char>
        vectordata = {entryData.begin(), entryData.end()};
        entry = vectordata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that data can be added in form of std::deque<unsigned char>
        dequedata = {entryData.begin(), entryData.end()};
        entry = dequedata;
        stringdata = entry;
        REQUIRE(entry.data<std::string>() == entryData);

        // ===== Check that the ZipEntry metadata are correct
        auto mdata = entry.metadata();
        REQUIRE(mdata.index() == 0);
        REQUIRE(mdata.compressedSize() == 0);
        REQUIRE(mdata.uncompressedSize() == entryData.size());
        REQUIRE(mdata.isDirectory() == false);
        REQUIRE(mdata.isEncrypted() == false);
        REQUIRE(mdata.isSupported() == true);
        REQUIRE(mdata.name() == "other_text_data.txt");
        REQUIRE(mdata.time() > 0);
    }
}

TEST_CASE("TEST 3: ZipArchive from Existing Archive on Disk") {

    // ===== Set up ZipArchive object and load the archive from disk
    KZip::ZipArchive archive("./CreatedWithWinZip.zip");
    REQUIRE(archive.isOpen());

    // ===== Check that the entry counts are correct.
    SECTION("#01: Count Entries in Archive") {
        REQUIRE(archive.entryCount(KZip::ZipFlags::Files) == 10);
        REQUIRE(archive.entryCount(KZip::ZipFlags::Directories) == 2);
        REQUIRE(archive.entryCount(KZip::ZipFlags::Files | KZip::ZipFlags::Directories) == 12);
    }

    // ===== Check that file names are correct
    SECTION("#02: Check File Names Exists") {
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

    // ===== Check that folder names are correct
    SECTION("#03: Check Folder Names Exists") {
        auto names = archive.entryNames(KZip::ZipFlags::Directories);
        REQUIRE(std::find(names.begin(), names.end(), "Folder 1/") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/") != names.end());
    }

    // ===== Check that all (folder and file) names are correct
    SECTION("#04: Check All Entry Names") {
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

    // ===== Check entry count in specific folder
    SECTION("#05: Count Number of Entries in Spscific Folder") {
        REQUIRE(archive.entryCount("Folder 2/") == 5);
    }

    SECTION("#06: Check Entry Names in Specific Folder") {
        auto names = archive.entryNames("Folder 2/");

        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 1.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 2.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 3.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 4.txt") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "Folder 2/file 5.txt") != names.end());
    }

    SECTION("#07: Check that All Entries Exists") {
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

    archive.close();
    REQUIRE_FALSE(archive.isOpen());
}

TEST_CASE("TEST 4: ZipEntry & ZipEntryProxy Created from Existing Archive") {

    std::string entryData = txtdata;

    // ===== Set up ZipArchive object and load the archive from disk
    KZip::ZipArchive archive("./CreatedWithWinZip.zip");
    REQUIRE(archive.isOpen());

    SECTION("#01: Check Contents of 'file 1.txt' using ZipEntryProxy") {
        REQUIRE(archive.entry("file 1.txt").name() == "file 1.txt");
        REQUIRE(archive.entry("file 1.txt") == txtdata);
        REQUIRE_FALSE(archive.entry("file 1.txt") == std::string("dummy string"));

        archive.entry("file 1.txt").setName("other file 1.txt");

        REQUIRE(archive.hasEntry("other file 1.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 1.txt"));
        REQUIRE(archive.entry("other file 1.txt").name() == "other file 1.txt");
        REQUIRE(archive.entry("other file 1.txt") == txtdata);
        REQUIRE_FALSE(archive.entry("other file 1.txt") == std::string("dummy string"));

        archive.save();
        archive.close();
        archive.open("./CreatedWithWinZip.zip");

        REQUIRE(archive.hasEntry("other file 1.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 1.txt"));
        REQUIRE(archive.entry("other file 1.txt").name() == "other file 1.txt");
        REQUIRE(archive.entry("other file 1.txt") == txtdata);
        REQUIRE_FALSE(archive.entry("other file 1.txt") == std::string("dummy string"));

    }



}

TEST_CASE("TEST 5: ADD & EXTRACT ENTRIES") {

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