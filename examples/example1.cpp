//
// Created by Troldal on 2019-03-10.
//

#include <iostream>
#include <Zippy.h>

using namespace std;

static const string str = "MISSION CONTROL I wouldn't worry too much about the computer. First of all, there is still a chance that he is right, despite your tests, and if it should happen again, we suggest eliminating this possibility by allowing the unit to remain in place and seeing whether or not it actually fails. If the computer should turn out to be wrong, the situation is still not alarming. The type of obsessional error he may be guilty of is not unknown among the latest generation of HAL 9000 computers. It has almost always revolved around a single detail, such as the one you have described, and it has never interfered with the integrity or reliability of the computer's performance in other areas. No one is certain of the cause of this kind of malfunctioning. It may be over-programming, but it could also be any number of reasons. In any event, it is somewhat analogous to human neurotic behavior. Does this answer your query?  Zero-five-three-Zero, MC, transmission concluded.";

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
