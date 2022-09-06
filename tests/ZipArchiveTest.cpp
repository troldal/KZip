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
        REQUIRE(archive.entry("file 2.txt").name() == "file 2.txt");
        REQUIRE(archive.entry("file 3.txt").name() == "file 3.txt");
        REQUIRE(archive.entry("file 4.txt").name() == "file 4.txt");
        REQUIRE(archive.entry("file 5.txt").name() == "file 5.txt");
        REQUIRE(archive.entry("Folder 2/file 1.txt").name() == "Folder 2/file 1.txt");
        REQUIRE(archive.entry("Folder 2/file 2.txt").name() == "Folder 2/file 2.txt");
        REQUIRE(archive.entry("Folder 2/file 3.txt").name() == "Folder 2/file 3.txt");
        REQUIRE(archive.entry("Folder 2/file 4.txt").name() == "Folder 2/file 4.txt");
        REQUIRE(archive.entry("Folder 2/file 5.txt").name() == "Folder 2/file 5.txt");

        REQUIRE_FALSE(archive.entry("file 1.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("file 2.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("file 3.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("file 4.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("file 5.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/file 1.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/file 2.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/file 3.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/file 4.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/file 5.txt").name() == "Folder 2/wrong name.txt");

        REQUIRE(archive.entry("file 1.txt") == txtdata);
        REQUIRE(archive.entry("file 2.txt") == txtdata);
        REQUIRE(archive.entry("file 3.txt") == txtdata);
        REQUIRE(archive.entry("file 4.txt") == txtdata);
        REQUIRE(archive.entry("file 5.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/file 1.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/file 2.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/file 3.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/file 4.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/file 5.txt") == txtdata);

        REQUIRE_FALSE(archive.entry("file 1.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("file 2.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("file 3.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("file 4.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("file 5.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/file 1.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/file 2.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/file 3.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/file 4.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/file 5.txt") == std::string("dummy string"));
    }

    SECTION("#02: Rename file entries using ZipEntryProxy") {
        REQUIRE_THROWS(archive.entry("file 1.txt").setName(""));
        REQUIRE_THROWS(archive.entry("file 2.txt").setName(""));
        REQUIRE_THROWS(archive.entry("file 3.txt").setName(""));
        REQUIRE_THROWS(archive.entry("file 4.txt").setName(""));
        REQUIRE_THROWS(archive.entry("file 5.txt").setName(""));
        REQUIRE_THROWS(archive.entry("Folder 2/file 1.txt").setName(""));
        REQUIRE_THROWS(archive.entry("Folder 2/file 2.txt").setName(""));
        REQUIRE_THROWS(archive.entry("Folder 2/file 3.txt").setName(""));
        REQUIRE_THROWS(archive.entry("Folder 2/file 4.txt").setName(""));
        REQUIRE_THROWS(archive.entry("Folder 2/file 5.txt").setName(""));

        REQUIRE_NOTHROW(archive.entry("file 1.txt").setName("file 1.txt"));
        REQUIRE_NOTHROW(archive.entry("file 2.txt").setName("file 2.txt"));
        REQUIRE_NOTHROW(archive.entry("file 3.txt").setName("file 3.txt"));
        REQUIRE_NOTHROW(archive.entry("file 4.txt").setName("file 4.txt"));
        REQUIRE_NOTHROW(archive.entry("file 5.txt").setName("file 5.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 1.txt").setName("Folder 2/file 1.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 2.txt").setName("Folder 2/file 2.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 3.txt").setName("Folder 2/file 3.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 4.txt").setName("Folder 2/file 4.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 5.txt").setName("Folder 2/file 5.txt"));

        REQUIRE(archive.hasEntry("file 1.txt"));
        REQUIRE(archive.hasEntry("file 2.txt"));
        REQUIRE(archive.hasEntry("file 3.txt"));
        REQUIRE(archive.hasEntry("file 4.txt"));
        REQUIRE(archive.hasEntry("file 5.txt"));
        REQUIRE(archive.hasEntry("Folder 2/file 1.txt"));
        REQUIRE(archive.hasEntry("Folder 2/file 2.txt"));
        REQUIRE(archive.hasEntry("Folder 2/file 3.txt"));
        REQUIRE(archive.hasEntry("Folder 2/file 4.txt"));
        REQUIRE(archive.hasEntry("Folder 2/file 5.txt"));

        REQUIRE_NOTHROW(archive.entry("file 1.txt").setName("other file 1.txt"));
        REQUIRE_NOTHROW(archive.entry("file 2.txt").setName("other file 2.txt"));
        REQUIRE_NOTHROW(archive.entry("file 3.txt").setName("other file 3.txt"));
        REQUIRE_NOTHROW(archive.entry("file 4.txt").setName("other file 4.txt"));
        REQUIRE_NOTHROW(archive.entry("file 5.txt").setName("other file 5.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 1.txt").setName("Folder 2/other file 1.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 2.txt").setName("Folder 2/other file 2.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 3.txt").setName("Folder 2/other file 3.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 4.txt").setName("Folder 2/other file 4.txt"));
        REQUIRE_NOTHROW(archive.entry("Folder 2/file 5.txt").setName("Folder 2/other file 5.txt"));

        REQUIRE(archive.hasEntry("other file 1.txt"));
        REQUIRE(archive.hasEntry("other file 2.txt"));
        REQUIRE(archive.hasEntry("other file 3.txt"));
        REQUIRE(archive.hasEntry("other file 4.txt"));
        REQUIRE(archive.hasEntry("other file 5.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 1.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 2.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 3.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 4.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 5.txt"));

        REQUIRE_FALSE(archive.hasEntry("file 1.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 2.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 3.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 4.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 5.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 1.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 2.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 3.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 4.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 5.txt"));

        REQUIRE(archive.entry("other file 1.txt").name() == "other file 1.txt");
        REQUIRE(archive.entry("other file 2.txt").name() == "other file 2.txt");
        REQUIRE(archive.entry("other file 3.txt").name() == "other file 3.txt");
        REQUIRE(archive.entry("other file 4.txt").name() == "other file 4.txt");
        REQUIRE(archive.entry("other file 5.txt").name() == "other file 5.txt");
        REQUIRE(archive.entry("Folder 2/other file 1.txt").name() == "Folder 2/other file 1.txt");
        REQUIRE(archive.entry("Folder 2/other file 2.txt").name() == "Folder 2/other file 2.txt");
        REQUIRE(archive.entry("Folder 2/other file 3.txt").name() == "Folder 2/other file 3.txt");
        REQUIRE(archive.entry("Folder 2/other file 4.txt").name() == "Folder 2/other file 4.txt");
        REQUIRE(archive.entry("Folder 2/other file 5.txt").name() == "Folder 2/other file 5.txt");

        REQUIRE_FALSE(archive.entry("other file 1.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("other file 2.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("other file 3.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("other file 4.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("other file 5.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 1.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 2.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 3.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 4.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 5.txt").name() == "Folder 2/wrong name.txt");

        REQUIRE(archive.entry("other file 1.txt") == txtdata);
        REQUIRE(archive.entry("other file 2.txt") == txtdata);
        REQUIRE(archive.entry("other file 3.txt") == txtdata);
        REQUIRE(archive.entry("other file 4.txt") == txtdata);
        REQUIRE(archive.entry("other file 5.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 1.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 2.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 3.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 4.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 5.txt") == txtdata);

        REQUIRE_FALSE(archive.entry("other file 1.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("other file 2.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("other file 3.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("other file 4.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("other file 5.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 1.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 2.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 3.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 4.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 5.txt") == std::string("dummy string"));

        REQUIRE_THROWS(archive.entry("other file 2.txt").setName("other file 1.txt"));
        REQUIRE_THROWS(archive.entry("other file 3.txt").setName("other file 1.txt"));
        REQUIRE_THROWS(archive.entry("other file 4.txt").setName("other file 1.txt"));
        REQUIRE_THROWS(archive.entry("other file 5.txt").setName("other file 1.txt"));
        REQUIRE_THROWS(archive.entry("Folder 2/other file 1.txt").setName("other file 1.txt"));
        REQUIRE_THROWS(archive.entry("Folder 2/other file 2.txt").setName("other file 1.txt"));
        REQUIRE_THROWS(archive.entry("Folder 2/other file 3.txt").setName("other file 1.txt"));
        REQUIRE_THROWS(archive.entry("Folder 2/other file 4.txt").setName("other file 1.txt"));
        REQUIRE_THROWS(archive.entry("Folder 2/other file 5.txt").setName("other file 1.txt"));

        archive.entry("other file 2.txt") = archive.entry("other file 1.txt");
        archive.entry("other file 3.txt") = archive.entry("other file 1.txt");
        archive.entry("other file 4.txt") = archive.entry("other file 1.txt");
        archive.entry("other file 5.txt") = archive.entry("other file 1.txt");
        archive.entry("Folder 2/other file 2.txt") = archive.entry("other file 1.txt");
        archive.entry("Folder 2/other file 2.txt") = archive.entry("other file 1.txt");
        archive.entry("Folder 2/other file 2.txt") = archive.entry("other file 1.txt");
        archive.entry("Folder 2/other file 2.txt") = archive.entry("other file 1.txt");

        REQUIRE(archive.entry("other file 1.txt") == archive.entry("other file 1.txt").data<std::string>());
        REQUIRE(archive.entry("other file 2.txt") == archive.entry("other file 1.txt").data<std::string>());
        REQUIRE(archive.entry("other file 3.txt") == archive.entry("other file 1.txt").data<std::string>());
        REQUIRE(archive.entry("other file 4.txt") == archive.entry("other file 1.txt").data<std::string>());
        REQUIRE(archive.entry("other file 5.txt") == archive.entry("other file 1.txt").data<std::string>());
        REQUIRE(archive.entry("Folder 2/other file 1.txt") == archive.entry("other file 1.txt").data<std::string>());
        REQUIRE(archive.entry("Folder 2/other file 2.txt") == archive.entry("other file 1.txt").data<std::string>());
        REQUIRE(archive.entry("Folder 2/other file 3.txt") == archive.entry("other file 1.txt").data<std::string>());
        REQUIRE(archive.entry("Folder 2/other file 4.txt") == archive.entry("other file 1.txt").data<std::string>());
        REQUIRE(archive.entry("Folder 2/other file 5.txt") == archive.entry("other file 1.txt").data<std::string>());

        archive.save("./CreatedWithWinZip_output.zip");
        archive.close();
        archive.open("./CreatedWithWinZip_output.zip");

        REQUIRE(archive.hasEntry("other file 1.txt"));
        REQUIRE(archive.hasEntry("other file 2.txt"));
        REQUIRE(archive.hasEntry("other file 3.txt"));
        REQUIRE(archive.hasEntry("other file 4.txt"));
        REQUIRE(archive.hasEntry("other file 5.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 1.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 2.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 3.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 4.txt"));
        REQUIRE(archive.hasEntry("Folder 2/other file 5.txt"));

        REQUIRE_FALSE(archive.hasEntry("file 1.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 2.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 3.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 4.txt"));
        REQUIRE_FALSE(archive.hasEntry("file 5.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 1.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 2.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 3.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 4.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 5.txt"));

        REQUIRE(archive.entry("other file 1.txt").name() == "other file 1.txt");
        REQUIRE(archive.entry("other file 2.txt").name() == "other file 2.txt");
        REQUIRE(archive.entry("other file 3.txt").name() == "other file 3.txt");
        REQUIRE(archive.entry("other file 4.txt").name() == "other file 4.txt");
        REQUIRE(archive.entry("other file 5.txt").name() == "other file 5.txt");
        REQUIRE(archive.entry("Folder 2/other file 1.txt").name() == "Folder 2/other file 1.txt");
        REQUIRE(archive.entry("Folder 2/other file 2.txt").name() == "Folder 2/other file 2.txt");
        REQUIRE(archive.entry("Folder 2/other file 3.txt").name() == "Folder 2/other file 3.txt");
        REQUIRE(archive.entry("Folder 2/other file 4.txt").name() == "Folder 2/other file 4.txt");
        REQUIRE(archive.entry("Folder 2/other file 5.txt").name() == "Folder 2/other file 5.txt");

        REQUIRE_FALSE(archive.entry("other file 1.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("other file 2.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("other file 3.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("other file 4.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("other file 5.txt").name() == "wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 1.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 2.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 3.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 4.txt").name() == "Folder 2/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder 2/other file 5.txt").name() == "Folder 2/wrong name.txt");

        REQUIRE(archive.entry("other file 1.txt") == txtdata);
        REQUIRE(archive.entry("other file 2.txt") == txtdata);
        REQUIRE(archive.entry("other file 3.txt") == txtdata);
        REQUIRE(archive.entry("other file 4.txt") == txtdata);
        REQUIRE(archive.entry("other file 5.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 1.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 2.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 3.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 4.txt") == txtdata);
        REQUIRE(archive.entry("Folder 2/other file 5.txt") == txtdata);

        REQUIRE_FALSE(archive.entry("other file 1.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("other file 2.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("other file 3.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("other file 4.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("other file 5.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 1.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 2.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 3.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 4.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder 2/other file 5.txt") == std::string("dummy string"));

        // ===== Cleanup...
        archive.close();
        archive.open("./CreatedWithWinZip.zip");
    }

    SECTION("#03: Rename folder entry using ZipEntryProxy") {

        archive.entry("Folder 2/").setName("Folder X/");

        REQUIRE(archive.hasEntry("Folder X/"));
        REQUIRE(archive.hasEntry("Folder X/file 1.txt"));
        REQUIRE(archive.hasEntry("Folder X/file 2.txt"));
        REQUIRE(archive.hasEntry("Folder X/file 3.txt"));
        REQUIRE(archive.hasEntry("Folder X/file 4.txt"));
        REQUIRE(archive.hasEntry("Folder X/file 5.txt"));

        REQUIRE_FALSE(archive.hasEntry("Folder 2/"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 1.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 2.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 3.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 4.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 5.txt"));

        REQUIRE(archive.entry("Folder X/file 1.txt").name() == "Folder X/file 1.txt");
        REQUIRE(archive.entry("Folder X/file 2.txt").name() == "Folder X/file 2.txt");
        REQUIRE(archive.entry("Folder X/file 3.txt").name() == "Folder X/file 3.txt");
        REQUIRE(archive.entry("Folder X/file 4.txt").name() == "Folder X/file 4.txt");
        REQUIRE(archive.entry("Folder X/file 5.txt").name() == "Folder X/file 5.txt");

        REQUIRE_FALSE(archive.entry("Folder X/file 1.txt").name() == "Folder X/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder X/file 2.txt").name() == "Folder X/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder X/file 3.txt").name() == "Folder X/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder X/file 4.txt").name() == "Folder X/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder X/file 5.txt").name() == "Folder X/wrong name.txt");

        REQUIRE(archive.entry("Folder X/file 1.txt") == txtdata);
        REQUIRE(archive.entry("Folder X/file 2.txt") == txtdata);
        REQUIRE(archive.entry("Folder X/file 3.txt") == txtdata);
        REQUIRE(archive.entry("Folder X/file 4.txt") == txtdata);
        REQUIRE(archive.entry("Folder X/file 5.txt") == txtdata);

        REQUIRE_FALSE(archive.entry("Folder X/file 1.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder X/file 2.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder X/file 3.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder X/file 4.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder X/file 5.txt") == std::string("dummy string"));

        archive.save("./CreatedWithWinZip_output.zip");
        archive.close();
        archive.open("./CreatedWithWinZip_output.zip");

        REQUIRE(archive.hasEntry("Folder X/"));
        REQUIRE(archive.hasEntry("Folder X/file 1.txt"));
        REQUIRE(archive.hasEntry("Folder X/file 2.txt"));
        REQUIRE(archive.hasEntry("Folder X/file 3.txt"));
        REQUIRE(archive.hasEntry("Folder X/file 4.txt"));
        REQUIRE(archive.hasEntry("Folder X/file 5.txt"));

        REQUIRE_FALSE(archive.hasEntry("Folder 2/"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 1.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 2.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 3.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 4.txt"));
        REQUIRE_FALSE(archive.hasEntry("Folder 2/file 5.txt"));

        REQUIRE(archive.entry("Folder X/file 1.txt").name() == "Folder X/file 1.txt");
        REQUIRE(archive.entry("Folder X/file 2.txt").name() == "Folder X/file 2.txt");
        REQUIRE(archive.entry("Folder X/file 3.txt").name() == "Folder X/file 3.txt");
        REQUIRE(archive.entry("Folder X/file 4.txt").name() == "Folder X/file 4.txt");
        REQUIRE(archive.entry("Folder X/file 5.txt").name() == "Folder X/file 5.txt");

        REQUIRE_FALSE(archive.entry("Folder X/file 1.txt").name() == "Folder X/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder X/file 2.txt").name() == "Folder X/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder X/file 3.txt").name() == "Folder X/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder X/file 4.txt").name() == "Folder X/wrong name.txt");
        REQUIRE_FALSE(archive.entry("Folder X/file 5.txt").name() == "Folder X/wrong name.txt");

        REQUIRE(archive.entry("Folder X/file 1.txt") == txtdata);
        REQUIRE(archive.entry("Folder X/file 2.txt") == txtdata);
        REQUIRE(archive.entry("Folder X/file 3.txt") == txtdata);
        REQUIRE(archive.entry("Folder X/file 4.txt") == txtdata);
        REQUIRE(archive.entry("Folder X/file 5.txt") == txtdata);

        REQUIRE_FALSE(archive.entry("Folder X/file 1.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder X/file 2.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder X/file 3.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder X/file 4.txt") == std::string("dummy string"));
        REQUIRE_FALSE(archive.entry("Folder X/file 5.txt") == std::string("dummy string"));

        // ===== Cleanup...
        archive.close();
        archive.open("./CreatedWithWinZip.zip");
    }

    SECTION("#04: Retrieve data using ZipEntryProxy") {

        // ===== Check that implicit conversion to std::string works as expected.
        std::string stringdata1 {};
        std::string stringdata2 {};
        std::string stringdata3 {};
        std::string stringdata4 {};
        std::string stringdata5 {};
        std::string stringdata6 {};
        std::string stringdata7 {};
        std::string stringdata8 {};
        std::string stringdata9 {};
        std::string stringdata10 {};

        stringdata1 = archive.entry("file 1.txt");
        stringdata2 = archive.entry("file 2.txt");
        stringdata3 = archive.entry("file 3.txt");
        stringdata4 = archive.entry("file 4.txt");
        stringdata5 = archive.entry("file 5.txt");
        stringdata6 = archive.entry("Folder 2/file 1.txt");
        stringdata7 = archive.entry("Folder 2/file 2.txt");
        stringdata8 = archive.entry("Folder 2/file 3.txt");
        stringdata9 = archive.entry("Folder 2/file 4.txt");
        stringdata10 = archive.entry("Folder 2/file 5.txt");

        REQUIRE(stringdata1 == entryData);
        REQUIRE(stringdata2 == entryData);
        REQUIRE(stringdata3 == entryData);
        REQUIRE(stringdata4 == entryData);
        REQUIRE(stringdata5 == entryData);
        REQUIRE(stringdata6 == entryData);
        REQUIRE(stringdata7 == entryData);
        REQUIRE(stringdata8 == entryData);
        REQUIRE(stringdata9 == entryData);
        REQUIRE(stringdata10 == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::vector<unsigned char> vectordata1 {};
        std::vector<unsigned char> vectordata2 {};
        std::vector<unsigned char> vectordata3 {};
        std::vector<unsigned char> vectordata4 {};
        std::vector<unsigned char> vectordata5 {};
        std::vector<unsigned char> vectordata6 {};
        std::vector<unsigned char> vectordata7 {};
        std::vector<unsigned char> vectordata8 {};
        std::vector<unsigned char> vectordata9 {};
        std::vector<unsigned char> vectordata10 {};

        vectordata1 = archive.entry("file 1.txt");
        vectordata2 = archive.entry("file 2.txt");
        vectordata3 = archive.entry("file 3.txt");
        vectordata4 = archive.entry("file 4.txt");
        vectordata5 = archive.entry("file 5.txt");
        vectordata6 = archive.entry("Folder 2/file 1.txt");
        vectordata7 = archive.entry("Folder 2/file 2.txt");
        vectordata8 = archive.entry("Folder 2/file 3.txt");
        vectordata9 = archive.entry("Folder 2/file 4.txt");
        vectordata10 = archive.entry("Folder 2/file 5.txt");

        REQUIRE(std::string{vectordata1.begin(), vectordata1.end()} == entryData);
        REQUIRE(std::string{vectordata2.begin(), vectordata2.end()} == entryData);
        REQUIRE(std::string{vectordata3.begin(), vectordata3.end()} == entryData);
        REQUIRE(std::string{vectordata4.begin(), vectordata4.end()} == entryData);
        REQUIRE(std::string{vectordata5.begin(), vectordata5.end()} == entryData);
        REQUIRE(std::string{vectordata6.begin(), vectordata6.end()} == entryData);
        REQUIRE(std::string{vectordata7.begin(), vectordata7.end()} == entryData);
        REQUIRE(std::string{vectordata8.begin(), vectordata8.end()} == entryData);
        REQUIRE(std::string{vectordata9.begin(), vectordata9.end()} == entryData);
        REQUIRE(std::string{vectordata10.begin(), vectordata10.end()} == entryData);

        // ===== Check that implicit conversion to std::vector<unsigned char> works as expected.
        std::deque<unsigned char> dequedata1 {};
        std::deque<unsigned char> dequedata2 {};
        std::deque<unsigned char> dequedata3 {};
        std::deque<unsigned char> dequedata4 {};
        std::deque<unsigned char> dequedata5 {};
        std::deque<unsigned char> dequedata6 {};
        std::deque<unsigned char> dequedata7 {};
        std::deque<unsigned char> dequedata8 {};
        std::deque<unsigned char> dequedata9 {};
        std::deque<unsigned char> dequedata10 {};

        dequedata1 = archive.entry("file 1.txt");
        dequedata2 = archive.entry("file 2.txt");
        dequedata3 = archive.entry("file 3.txt");
        dequedata4 = archive.entry("file 4.txt");
        dequedata5 = archive.entry("file 5.txt");
        dequedata6 = archive.entry("Folder 2/file 1.txt");
        dequedata7 = archive.entry("Folder 2/file 2.txt");
        dequedata8 = archive.entry("Folder 2/file 3.txt");
        dequedata9 = archive.entry("Folder 2/file 4.txt");
        dequedata10 = archive.entry("Folder 2/file 5.txt");

        REQUIRE(std::string{dequedata1.begin(), dequedata1.end()} == entryData);
        REQUIRE(std::string{dequedata2.begin(), dequedata2.end()} == entryData);
        REQUIRE(std::string{dequedata3.begin(), dequedata3.end()} == entryData);
        REQUIRE(std::string{dequedata4.begin(), dequedata4.end()} == entryData);
        REQUIRE(std::string{dequedata5.begin(), dequedata5.end()} == entryData);
        REQUIRE(std::string{dequedata6.begin(), dequedata6.end()} == entryData);
        REQUIRE(std::string{dequedata7.begin(), dequedata7.end()} == entryData);
        REQUIRE(std::string{dequedata8.begin(), dequedata8.end()} == entryData);
        REQUIRE(std::string{dequedata9.begin(), dequedata9.end()} == entryData);
        REQUIRE(std::string{dequedata10.begin(), dequedata10.end()} == entryData);

        // ===== Check that data can be added in form of std::vector<unsigned char>
        std::vector<unsigned char> vectordata = {entryData.begin(), entryData.end()};

        archive.entry("file 1.txt") = vectordata;
        archive.entry("file 2.txt") = vectordata;
        archive.entry("file 3.txt") = vectordata;
        archive.entry("file 4.txt") = vectordata;
        archive.entry("file 5.txt") = vectordata;
        archive.entry("Folder 2/file 1.txt") = vectordata;
        archive.entry("Folder 2/file 2.txt") = vectordata;
        archive.entry("Folder 2/file 3.txt") = vectordata;
        archive.entry("Folder 2/file 4.txt") = vectordata;
        archive.entry("Folder 2/file 5.txt") = vectordata;

        stringdata1 = archive.entry("file 1.txt");
        stringdata2 = archive.entry("file 2.txt");
        stringdata3 = archive.entry("file 3.txt");
        stringdata4 = archive.entry("file 4.txt");
        stringdata5 = archive.entry("file 5.txt");
        stringdata6 = archive.entry("Folder 2/file 1.txt");
        stringdata7 = archive.entry("Folder 2/file 2.txt");
        stringdata8 = archive.entry("Folder 2/file 3.txt");
        stringdata9 = archive.entry("Folder 2/file 4.txt");
        stringdata10 = archive.entry("Folder 2/file 5.txt");

        REQUIRE(archive.entry("file 1.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("file 2.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("file 3.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("file 4.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("file 5.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 1.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 2.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 3.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 4.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 5.txt").data<std::string>() == entryData);

        // ===== Check that data can be added in form of std::deque<unsigned char>
        std::deque<unsigned char> dequedata = {entryData.begin(), entryData.end()};

        archive.entry("file 1.txt") = dequedata;
        archive.entry("file 2.txt") = dequedata;
        archive.entry("file 3.txt") = dequedata;
        archive.entry("file 4.txt") = dequedata;
        archive.entry("file 5.txt") = dequedata;
        archive.entry("Folder 2/file 1.txt") = dequedata;
        archive.entry("Folder 2/file 2.txt") = dequedata;
        archive.entry("Folder 2/file 3.txt") = dequedata;
        archive.entry("Folder 2/file 4.txt") = dequedata;
        archive.entry("Folder 2/file 5.txt") = dequedata;

        stringdata1 = archive.entry("file 1.txt");
        stringdata2 = archive.entry("file 2.txt");
        stringdata3 = archive.entry("file 3.txt");
        stringdata4 = archive.entry("file 4.txt");
        stringdata5 = archive.entry("file 5.txt");
        stringdata6 = archive.entry("Folder 2/file 1.txt");
        stringdata7 = archive.entry("Folder 2/file 2.txt");
        stringdata8 = archive.entry("Folder 2/file 3.txt");
        stringdata9 = archive.entry("Folder 2/file 4.txt");
        stringdata10 = archive.entry("Folder 2/file 5.txt");

        REQUIRE(archive.entry("file 1.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("file 2.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("file 3.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("file 4.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("file 5.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 1.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 2.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 3.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 4.txt").data<std::string>() == entryData);
        REQUIRE(archive.entry("Folder 2/file 5.txt").data<std::string>() == entryData);

        // ===== Check that the ZipEntry metadata are correct
        auto mdata1 = archive.entry("file 1.txt").metadata();
        auto mdata2 = archive.entry("file 2.txt").metadata();
        auto mdata3 = archive.entry("file 3.txt").metadata();
        auto mdata4 = archive.entry("file 4.txt").metadata();
        auto mdata5 = archive.entry("file 5.txt").metadata();
        auto mdata6 = archive.entry("Folder 2/file 1.txt").metadata();
        auto mdata7 = archive.entry("Folder 2/file 2.txt").metadata();
        auto mdata8 = archive.entry("Folder 2/file 3.txt").metadata();
        auto mdata9 = archive.entry("Folder 2/file 4.txt").metadata();
        auto mdata10 = archive.entry("Folder 2/file 5.txt").metadata();

        REQUIRE(mdata1.index() >= 0);
        REQUIRE(mdata2.index() >= 0);
        REQUIRE(mdata3.index() >= 0);
        REQUIRE(mdata4.index() >= 0);
        REQUIRE(mdata5.index() >= 0);
        REQUIRE(mdata6.index() >= 0);
        REQUIRE(mdata7.index() >= 0);
        REQUIRE(mdata8.index() >= 0);
        REQUIRE(mdata9.index() >= 0);
        REQUIRE(mdata10.index() >= 0);

        REQUIRE(mdata1.compressedSize() != 0);
        REQUIRE(mdata2.compressedSize() != 0);
        REQUIRE(mdata3.compressedSize() != 0);
        REQUIRE(mdata4.compressedSize() != 0);
        REQUIRE(mdata5.compressedSize() != 0);
        REQUIRE(mdata6.compressedSize() != 0);
        REQUIRE(mdata7.compressedSize() != 0);
        REQUIRE(mdata8.compressedSize() != 0);
        REQUIRE(mdata9.compressedSize() != 0);
        REQUIRE(mdata10.compressedSize() != 0);

        REQUIRE(mdata1.uncompressedSize() == entryData.size());
        REQUIRE(mdata2.uncompressedSize() == entryData.size());
        REQUIRE(mdata3.uncompressedSize() == entryData.size());
        REQUIRE(mdata4.uncompressedSize() == entryData.size());
        REQUIRE(mdata5.uncompressedSize() == entryData.size());
        REQUIRE(mdata6.uncompressedSize() == entryData.size());
        REQUIRE(mdata7.uncompressedSize() == entryData.size());
        REQUIRE(mdata8.uncompressedSize() == entryData.size());
        REQUIRE(mdata9.uncompressedSize() == entryData.size());
        REQUIRE(mdata10.uncompressedSize() == entryData.size());

        REQUIRE(mdata1.isDirectory() == false);
        REQUIRE(mdata2.isDirectory() == false);
        REQUIRE(mdata3.isDirectory() == false);
        REQUIRE(mdata4.isDirectory() == false);
        REQUIRE(mdata5.isDirectory() == false);
        REQUIRE(mdata6.isDirectory() == false);
        REQUIRE(mdata7.isDirectory() == false);
        REQUIRE(mdata8.isDirectory() == false);
        REQUIRE(mdata9.isDirectory() == false);
        REQUIRE(mdata10.isDirectory() == false);

        REQUIRE(mdata1.isEncrypted() == false);
        REQUIRE(mdata2.isEncrypted() == false);
        REQUIRE(mdata3.isEncrypted() == false);
        REQUIRE(mdata4.isEncrypted() == false);
        REQUIRE(mdata5.isEncrypted() == false);
        REQUIRE(mdata6.isEncrypted() == false);
        REQUIRE(mdata7.isEncrypted() == false);
        REQUIRE(mdata8.isEncrypted() == false);
        REQUIRE(mdata9.isEncrypted() == false);
        REQUIRE(mdata10.isEncrypted() == false);

        REQUIRE(mdata1.isSupported() == true);
        REQUIRE(mdata2.isSupported() == true);
        REQUIRE(mdata3.isSupported() == true);
        REQUIRE(mdata4.isSupported() == true);
        REQUIRE(mdata5.isSupported() == true);
        REQUIRE(mdata6.isSupported() == true);
        REQUIRE(mdata7.isSupported() == true);
        REQUIRE(mdata8.isSupported() == true);
        REQUIRE(mdata9.isSupported() == true);
        REQUIRE(mdata10.isSupported() == true);

        REQUIRE(mdata1.name() == "file 1.txt");
        REQUIRE(mdata2.name() == "file 2.txt");
        REQUIRE(mdata3.name() == "file 3.txt");
        REQUIRE(mdata4.name() == "file 4.txt");
        REQUIRE(mdata5.name() == "file 5.txt");
        REQUIRE(mdata6.name() == "Folder 2/file 1.txt");
        REQUIRE(mdata7.name() == "Folder 2/file 2.txt");
        REQUIRE(mdata8.name() == "Folder 2/file 3.txt");
        REQUIRE(mdata9.name() == "Folder 2/file 4.txt");
        REQUIRE(mdata10.name() == "Folder 2/file 5.txt");

        REQUIRE(mdata1.time() > 0);
        REQUIRE(mdata2.time() > 0);
        REQUIRE(mdata3.time() > 0);
        REQUIRE(mdata4.time() > 0);
        REQUIRE(mdata5.time() > 0);
        REQUIRE(mdata6.time() > 0);
        REQUIRE(mdata7.time() > 0);
        REQUIRE(mdata8.time() > 0);
        REQUIRE(mdata9.time() > 0);
        REQUIRE(mdata10.time() > 0);
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