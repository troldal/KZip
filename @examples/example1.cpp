//
// Created by Troldal on 2019-03-10.
//

#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <Zippy/Zippy.h>
#include "tableprinter.h"

using namespace std;

static const string str = "MISSION CONTROL I wouldn't worry too much about the computer. First of all, there is still a chance that he is right, despite your tests, and if it should happen again, we suggest eliminating this possibility by allowing the unit to remain in place and seeing whether or not it actually fails. If the computer should turn out to be wrong, the situation is still not alarming. The type of obsessional error he may be guilty of is not unknown among the latest generation of HAL 9000 computers. It has almost always revolved around a single detail, such as the one you have described, and it has never interfered with the integrity or reliability of the computer's performance in other areas. No one is certain of the cause of this kind of malfunctioning. It may be over-programming, but it could also be any number of reasons. In any event, it is somewhat analogous to human neurotic behavior. Does this answer your query?  Zero-five-three-Zero, MC, transmission concluded.";

int main() {

    cout << "Creating new archive..." << endl;

    Zippy::ZipArchive arch;
    arch.Create("./TestArchive.zip");

    cout << "Adding 50 files to archive..." << endl;
    for (int i = 0; i < 50; ++i) {
        if (i < 10)
            arch.AddEntry("0" + to_string(i) + ".txt", "this is test 0" + to_string(i) + ": " + str);
        else
            arch.AddEntry(to_string(i) + ".txt", "this is test " + to_string(i) + ": " + str);
    }

    arch.Save();

    cout << "Deleting 25 files from archive..." << endl;
    for (int i = 0; i < 25; ++i) {
        if (i < 10)
            arch.DeleteEntry("0" + to_string(i) + ".txt");
        else
            arch.DeleteEntry(to_string(i) + ".txt");
    }

    arch.Save();

    cout << "Contents of archive:" << endl;
    TablePrinter::TablePrinter tp(&std::cout);
    tp.AddColumn("Index", 8);
    tp.AddColumn("Name", 8);
    tp.AddColumn("Compressed Size:", 20);
    tp.AddColumn("Uncompressed Size:", 20);
    tp.AddColumn("Date Stamp:", 25);

    tp.PrintHeader();
    for (auto& item : arch.GetMetaData()) {
        std::string time = std::ctime(&item.Time);
        time = time.substr(0, time.length() - 1);

        tp << (item.Index < 10 ? "0" + to_string(item.Index) : to_string(item.Index))
           << item.Filename
           << item.CompressedSize
           << item.UncompressedSize
           << time;
    }
    tp.PrintFooter();

    cout << endl << "Contents of file \"49.txt\":" << endl;
    auto entry = arch.GetEntry("49.txt");
    cout << entry.GetDataAsString() << endl;

}
