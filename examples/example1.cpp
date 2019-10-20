//
// Created by Troldal on 2019-03-10.
//

#include <iostream>
#include <Zippy.h>

using namespace std;

static const string str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Magnis dis parturient montes nascetur ridiculus mus mauris vitae ultricies. At quis risus sed vulputate. Id neque aliquam vestibulum morbi blandit cursus risus at. Velit egestas dui id ornare arcu. Facilisis magna etiam tempor orci eu lobortis. Nisi quis eleifend quam adipiscing vitae proin sagittis nisl rhoncus. Quis risus sed vulputate odio ut enim blandit. Imperdiet dui accumsan sit amet nulla. Eget mauris pharetra et ultrices neque ornare aenean. Ut venenatis tellus in metus vulputate eu scelerisque. Vel turpis nunc eget lorem. In ante metus dictum at tempor commodo ullamcorper a lacus. Convallis posuere morbi leo urna molestie at elementum eu. Augue ut lectus arcu bibendum at varius. Iaculis urna id volutpat lacus laoreet non. Fames ac turpis egestas maecenas pharetra.";

int main() {

    // ===== Creating empty archive
    Zippy::ZipArchive arch;
    arch.Create("./TestArchive.zip");

    // ===== Adding 10 entries to the archive
    for (int i = 0; i <= 9; ++i)
        arch.AddEntry(to_string(i) + ".txt", "this is test " + to_string(i) + ": " + str);

    // ===== Delete the first entry
    arch.DeleteEntry("0.txt");

    // ===== Save and close the archive
    arch.Save();
    arch.Close();

    // ===== Reopen and check contents
    arch.Open("./TestArchive.zip");
    cout << "Number of entries in archive: " << arch.GetNumEntries() << endl;
    cout << "Content of \"9.txt\": " << endl << arch.GetEntry("9.txt").GetDataAsString();

    return 0;
}
