//
// Created by Troldal on 2019-03-13.
//

#include <catch.hpp>
#include <Zippy.hpp>
#include "test-data-text.hpp"
#include "test-data-binary.hpp"

// Add binary file to archive
// Add folders to archive
// Open existing archive
// List contents in archive

TEST_CASE("TEST 1: ARCHIVE CREATION") {




    /**
     * @brief Test if an empty ZipArchive object can be created, without an associated archive file.
     */
    SECTION("Create empty archive object") {
        Zippy::ZipArchive archive;
        REQUIRE(!archive.IsOpen());
    }

        /**
         * @brief Test that trying to do operations on an empty archive object will throw an exception.
         */
    SECTION("Throws when trying to modify an empty archive object") {
        Zippy::ZipArchive archive;
        REQUIRE_THROWS(archive.GetEntryNames());
        REQUIRE_THROWS(archive.GetEntryNamesInDir(""));
        REQUIRE_THROWS(archive.GetMetaData());
        REQUIRE_THROWS(archive.GetMetaDataInDir(""));
        REQUIRE_THROWS(archive.GetNumEntries());
        REQUIRE_THROWS(archive.GetNumEntriesInDir(""));
        REQUIRE_THROWS(archive.HasEntry(""));
        REQUIRE_THROWS(archive.Save());
        REQUIRE_THROWS(archive.DeleteEntry(""));
        REQUIRE_THROWS(archive.GetEntry(""));
        REQUIRE_THROWS(archive.ExtractEntry("", ""));
        REQUIRE_THROWS(archive.ExtractDir("", ""));
        REQUIRE_THROWS(archive.ExtractAll(""));
        REQUIRE_THROWS(archive.AddEntry("Entry.txt", "TEST"));
    }

        /**
         * @brief
         */
    SECTION("Create empty archive file") {
        Zippy::ZipArchive archive;
        archive.Create("TestArchive.zip");
        REQUIRE(archive.IsOpen());
        REQUIRE(archive.GetEntryNames().empty());
        REQUIRE(archive.GetEntryNamesInDir("").empty());
        REQUIRE(archive.GetMetaData().empty());
        REQUIRE(archive.GetMetaDataInDir("").empty());
        REQUIRE(archive.GetNumEntries() == 0);
        REQUIRE(archive.GetNumEntriesInDir("") == 0);
    }

}

TEST_CASE("TEST 2: ADD DATA TO ARCHIVE") {

    /**
     * @brief
     */
    SECTION("Add text data to archive") {
        Zippy::ZipArchive archive;
        std::string archivePath = "./TestArchive.zip";
        std::string entryName = "text_data.txt";
        std::string entryData = txtdata;

        archive.Create(archivePath);
        archive.AddEntry(entryName, entryData);
        REQUIRE(archive.IsOpen());
        REQUIRE(archive.GetEntryNames().size() == 1);
        REQUIRE(archive.GetEntryNames().at(0) == entryName);
        REQUIRE(archive.GetEntryNamesInDir("").size() == 1);
        REQUIRE(archive.GetEntryNamesInDir("").at(0) == entryName);
        REQUIRE(archive.GetMetaData().size() == 1);
        REQUIRE(archive.GetMetaData().at(0).Filename == entryName);
        REQUIRE(archive.GetMetaDataInDir("").size() == 1);
        REQUIRE(archive.GetMetaDataInDir("").at(0).Filename == entryName);
        REQUIRE(archive.GetNumEntries() == 1);
        REQUIRE(archive.GetNumEntriesInDir("") == 1);
        REQUIRE(archive.HasEntry(entryName));
        REQUIRE(archive.GetEntry(entryName).GetDataAsString() == entryData);

        archive.Save();
        archive.Close();
        archive.Open(archivePath);
        REQUIRE(archive.IsOpen());
        REQUIRE(archive.GetEntryNames().size() == 1);
        REQUIRE(archive.GetEntryNames().at(0) == entryName);
        REQUIRE(archive.GetEntryNamesInDir("").size() == 1);
        REQUIRE(archive.GetEntryNamesInDir("").at(0) == entryName);
        REQUIRE(archive.GetMetaData().size() == 1);
        REQUIRE(archive.GetMetaData().at(0).Filename == entryName);
        REQUIRE(archive.GetMetaDataInDir("").size() == 1);
        REQUIRE(archive.GetMetaDataInDir("").at(0).Filename == entryName);
        REQUIRE(archive.GetNumEntries() == 1);
        REQUIRE(archive.GetNumEntriesInDir("") == 1);
        REQUIRE(archive.HasEntry(entryName));
        REQUIRE(archive.GetEntry(entryName).GetDataAsString() == entryData);
    }

    /**
     * @brief
     */
    SECTION("Add binary data to archive") {
        Zippy::ZipArchive archive;
        std::string archivePath = "./TestArchive.zip";
        std::string entryName = "binary_data.png";
        auto entryData = bindata;

        archive.Create(archivePath);
        archive.AddEntry(entryName, entryData);
        REQUIRE(archive.IsOpen());
        REQUIRE(archive.GetEntryNames().size() == 1);
        REQUIRE(archive.GetEntryNames().at(0) == entryName);
        REQUIRE(archive.GetEntryNamesInDir("").size() == 1);
        REQUIRE(archive.GetEntryNamesInDir("").at(0) == entryName);
        REQUIRE(archive.GetMetaData().size() == 1);
        REQUIRE(archive.GetMetaData().at(0).Filename == entryName);
        REQUIRE(archive.GetMetaDataInDir("").size() == 1);
        REQUIRE(archive.GetMetaDataInDir("").at(0).Filename == entryName);
        REQUIRE(archive.GetNumEntries() == 1);
        REQUIRE(archive.GetNumEntriesInDir("") == 1);
        REQUIRE(archive.HasEntry(entryName));
        REQUIRE(archive.GetEntry(entryName).GetData() == entryData);

        archive.Save();
        archive.Close();
        archive.Open(archivePath);
        REQUIRE(archive.IsOpen());
        REQUIRE(archive.GetEntryNames().size() == 1);
        REQUIRE(archive.GetEntryNames().at(0) == entryName);
        REQUIRE(archive.GetEntryNamesInDir("").size() == 1);
        REQUIRE(archive.GetEntryNamesInDir("").at(0) == entryName);
        REQUIRE(archive.GetMetaData().size() == 1);
        REQUIRE(archive.GetMetaData().at(0).Filename == entryName);
        REQUIRE(archive.GetMetaDataInDir("").size() == 1);
        REQUIRE(archive.GetMetaDataInDir("").at(0).Filename == entryName);
        REQUIRE(archive.GetNumEntries() == 1);
        REQUIRE(archive.GetNumEntriesInDir("") == 1);
        REQUIRE(archive.HasEntry(entryName));
        REQUIRE(archive.GetEntry(entryName).GetData() == entryData);
    }
}





// =============


TEST_CASE("Test 2: Open existing archive") {

    // Set up
    Zippy::ZipArchive archive;
    archive.Open("./CreatedWithWinZip.zip");

    SECTION("Count entries") {
        REQUIRE(archive.GetNumEntries() == 12);
    }

    SECTION("List entries") {
        auto names = archive.GetEntryNames();
        REQUIRE(names[0] == "file 1.txt");
        REQUIRE(names[1] == "file 2.txt");
        REQUIRE(names[2] == "file 3.txt");
        REQUIRE(names[3] == "file 4.txt");
        REQUIRE(names[4] == "file 5.txt");
        REQUIRE(names[5] == "Folder 1/");
        REQUIRE(names[6] == "Folder 2/");
        REQUIRE(names[7] == "Folder 2/file 1.txt");
        REQUIRE(names[8] == "Folder 2/file 2.txt");
        REQUIRE(names[9] == "Folder 2/file 3.txt");
        REQUIRE(names[10] == "Folder 2/file 4.txt");
        REQUIRE(names[11] == "Folder 2/file 5.txt");
    }

    SECTION("Count entries in directory") {
        REQUIRE(archive.GetNumEntriesInDir("Folder 2/") == 5);
    }

    SECTION("List entries in directory") {
        auto names = archive.GetEntryNamesInDir("Folder 2/");

        REQUIRE(names[0] == "Folder 2/file 1.txt");
        REQUIRE(names[1] == "Folder 2/file 2.txt");
        REQUIRE(names[2] == "Folder 2/file 3.txt");
        REQUIRE(names[3] == "Folder 2/file 4.txt");
        REQUIRE(names[4] == "Folder 2/file 5.txt");
    }

    SECTION("Check entries exist") {
        REQUIRE(archive.HasEntry("file 1.txt"));
        REQUIRE(archive.HasEntry("file 2.txt"));
        REQUIRE(archive.HasEntry("file 3.txt"));
        REQUIRE(archive.HasEntry("file 4.txt"));
        REQUIRE(archive.HasEntry("file 5.txt"));
        REQUIRE(archive.HasEntry("Folder 1/"));
        REQUIRE(archive.HasEntry("Folder 2/"));
        REQUIRE(archive.HasEntry("Folder 2/file 1.txt"));
        REQUIRE(archive.HasEntry("Folder 2/file 2.txt"));
        REQUIRE(archive.HasEntry("Folder 2/file 3.txt"));
        REQUIRE(archive.HasEntry("Folder 2/file 4.txt"));
        REQUIRE(archive.HasEntry("Folder 2/file 5.txt"));
    }

    SECTION("Extract entry to disk") {
        REQUIRE(false);
    }

    SECTION("Extract directory to disk") {
        REQUIRE(false);
    }

    SECTION("Extract all to disk") {
        REQUIRE(false);
    }

    SECTION("Get entry binary data") {
        auto result = archive.GetEntry("file 1.txt").GetData();
        std::string resultStr;
        for (auto& ch : result)
            resultStr += static_cast<char>(ch);

        std::string compare = "MISSION CONTROL I wouldn't worry too much about the computer. First of all, there is still a chance that he is right, despite your tests, and if it should happen again, we suggest eliminating this possibility by allowing the unit to remain in place and seeing whether or not it actually fails. If the computer should turn out to be wrong, the situation is still not alarming. The type of obsessional error he may be guilty of is not unknown among the latest generation of HAL 9000 computers. It has almost always revolved around a single detail, such as the one you have described, and it has never interfered with the integrity or reliability of the computer's performance in other areas. No one is certain of the cause of this kind of malfunctioning. It may be over-programming, but it could also be any number of reasons. In any event, it is somewhat analogous to human neurotic behavior. Does this answer your query?  Zero-five-three-Zero, MC, transmission concluded.";
        REQUIRE(resultStr == compare);
    }

    SECTION("Get entry string data") {
        std::string result = archive.GetEntry("file 1.txt").GetDataAsString();
        std::string compare = "MISSION CONTROL I wouldn't worry too much about the computer. First of all, there is still a chance that he is right, despite your tests, and if it should happen again, we suggest eliminating this possibility by allowing the unit to remain in place and seeing whether or not it actually fails. If the computer should turn out to be wrong, the situation is still not alarming. The type of obsessional error he may be guilty of is not unknown among the latest generation of HAL 9000 computers. It has almost always revolved around a single detail, such as the one you have described, and it has never interfered with the integrity or reliability of the computer's performance in other areas. No one is certain of the cause of this kind of malfunctioning. It may be over-programming, but it could also be any number of reasons. In any event, it is somewhat analogous to human neurotic behavior. Does this answer your query?  Zero-five-three-Zero, MC, transmission concluded.";
        REQUIRE(result == compare);
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