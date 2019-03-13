//
// Created by Troldal on 2019-03-10.
//

#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include "../@library/ZipArchive.h"

using namespace std;

int main() {

    SimpleZip::ZipArchive arch("./BenchmarkString.xlsx");

    cout << "This .zip archive has " << arch.GetNumEntries() << " entries. They are:" << endl << endl;

    auto& entry = arch.GetEntry("xl/workbook.xml");

    cout << entry.GetDataAsString() << endl;

    //This causes a crash!
    //auto blah = std::move(entry);

    arch.AddEntry("blah.xml", "BlahBlahBlah");
    arch.Save();

    cout << "This .zip archive has " << arch.GetNumEntries() << " entries. They are:" << endl << endl;

    for (auto& item : arch.GetEntryNames())
        cout << item << endl;

    cout << std::ctime(&arch.GetEntry("docProps/app.xml").Time()) << endl;
}