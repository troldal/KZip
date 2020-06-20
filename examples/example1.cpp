//
// Created by Troldal on 2019-03-10.
//

#include <iostream>
#include <Zippy.hpp>
#include "tableprinter.h"
#include "image.h"

using namespace std;

// ===== Items to put in the test archive
const string str0 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Tortor consequat id porta nibh venenatis cras. Justo nec ultrices dui sapien. Ultricies lacus sed turpis tincidunt id. Fames ac turpis egestas integer eget aliquet nibh praesent tristique. Tortor id aliquet lectus proin. Sit amet est placerat in egestas erat imperdiet. Ut etiam sit amet nisl purus in mollis nunc sed. Magnis dis parturient montes nascetur. Adipiscing diam donec adipiscing tristique risus. Purus sit amet luctus venenatis. Ut etiam sit amet nisl purus.";
const string str1 = "Orci dapibus ultrices in iaculis. Quam elementum pulvinar etiam non quam lacus suspendisse. Vestibulum rhoncus est pellentesque elit ullamcorper dignissim cras tincidunt. Aliquet nec ullamcorper sit amet risus nullam eget felis. Condimentum vitae sapien pellentesque habitant morbi tristique senectus et netus. Pellentesque habitant morbi tristique senectus et netus et malesuada. Pellentesque habitant morbi tristique senectus et netus et malesuada fames. Amet consectetur adipiscing elit duis tristique sollicitudin. Aliquam eleifend mi in nulla posuere sollicitudin. Vulputate mi sit amet mauris commodo quis. Pellentesque elit eget gravida cum sociis natoque penatibus. Mauris rhoncus aenean vel elit scelerisque mauris. Duis tristique sollicitudin nibh sit amet commodo nulla facilisi nullam. Viverra accumsan in nisl nisi.";
const string str2 = "Consequat mauris nunc congue nisi vitae suscipit. Massa tincidunt dui ut ornare lectus sit. At tellus at urna condimentum mattis. Turpis nunc eget lorem dolor sed. Nam aliquam sem et tortor consequat. Sit amet nisl suscipit adipiscing. Senectus et netus et malesuada fames ac turpis. In hendrerit gravida rutrum quisque non tellus. Sem nulla pharetra diam sit. Sed adipiscing diam donec adipiscing.";
const string str3 = "Fames ac turpis egestas maecenas pharetra. Mi quis hendrerit dolor magna eget est. Lacus vel facilisis volutpat est velit egestas dui id ornare. Eu tincidunt tortor aliquam nulla facilisi cras fermentum odio. Tempor orci dapibus ultrices in iaculis nunc sed. Adipiscing bibendum est ultricies integer. Amet consectetur adipiscing elit duis tristique sollicitudin. Nullam non nisi est sit. Eget nulla facilisi etiam dignissim diam quis enim lobortis. Volutpat est velit egestas dui id ornare arcu odio. Elementum curabitur vitae nunc sed velit dignissim sodales. Sollicitudin ac orci phasellus egestas tellus rutrum tellus pellentesque eu. Dui vivamus arcu felis bibendum ut.";
const string str4 = "Netus et malesuada fames ac. Elit scelerisque mauris pellentesque pulvinar pellentesque habitant morbi tristique. At auctor urna nunc id cursus. Dolor sed viverra ipsum nunc. Consectetur purus ut faucibus pulvinar. Et netus et malesuada fames ac. Imperdiet proin fermentum leo vel orci porta non pulvinar neque. Feugiat pretium nibh ipsum consequat nisl vel. Nibh sed pulvinar proin gravida hendrerit lectus. Etiam erat velit scelerisque in dictum non consectetur a. Vel risus commodo viverra maecenas accumsan lacus vel facilisis. Aliquam vestibulum morbi blandit cursus. Gravida rutrum quisque non tellus orci ac auctor. Posuere ac ut consequat semper viverra nam libero justo laoreet. Etiam non quam lacus suspendisse faucibus. Elit eget gravida cum sociis.";
const string str5 = "Elementum facilisis leo vel fringilla est ullamcorper. Dui ut ornare lectus sit amet est. Turpis in eu mi bibendum neque. Odio pellentesque diam volutpat commodo sed. Bibendum ut tristique et egestas. Pellentesque massa placerat duis ultricies lacus sed turpis. Mi in nulla posuere sollicitudin aliquam ultrices. Cras fermentum odio eu feugiat pretium nibh ipsum consequat. Metus vulputate eu scelerisque felis imperdiet proin. Massa eget egestas purus viverra accumsan in nisl. Sit amet tellus cras adipiscing. Tristique magna sit amet purus gravida.";
const string str6 = "Amet nisl suscipit adipiscing bibendum est ultricies integer. Proin gravida hendrerit lectus a. Ipsum dolor sit amet consectetur. Duis convallis convallis tellus id interdum velit. Pellentesque massa placerat duis ultricies lacus sed turpis tincidunt id. Rhoncus aenean vel elit scelerisque mauris pellentesque pulvinar. Id porta nibh venenatis cras sed felis eget. Justo eget magna fermentum iaculis eu non diam phasellus. Nisi est sit amet facilisis magna etiam tempor orci eu. Morbi enim nunc faucibus a. Ultricies mi eget mauris pharetra et ultrices. Urna porttitor rhoncus dolor purus non. Porta nibh venenatis cras sed felis eget velit aliquet sagittis. Eget velit aliquet sagittis id consectetur purus ut. In nisl nisi scelerisque eu ultrices vitae. Scelerisque felis imperdiet proin fermentum leo vel. Nunc non blandit massa enim nec dui. At elementum eu facilisis sed odio. Tristique senectus et netus et malesuada fames. Neque aliquam vestibulum morbi blandit cursus.";
const string str7 = "Tempus imperdiet nulla malesuada pellentesque elit. Adipiscing elit duis tristique sollicitudin nibh sit amet. Lectus proin nibh nisl condimentum id venenatis a condimentum vitae. Habitant morbi tristique senectus et netus et malesuada. Lectus mauris ultrices eros in. Diam in arcu cursus euismod quis. Mauris sit amet massa vitae tortor. Porta nibh venenatis cras sed. Vel pretium lectus quam id leo in. In mollis nunc sed id semper risus in hendrerit gravida. Scelerisque fermentum dui faucibus in ornare quam viverra.";
const string str8 = "Augue eget arcu dictum varius. Senectus et netus et malesuada fames. Aliquam eleifend mi in nulla posuere sollicitudin aliquam. Semper auctor neque vitae tempus quam pellentesque nec. Vulputate sapien nec sagittis aliquam malesuada bibendum arcu vitae elementum. Donec pretium vulputate sapien nec sagittis. Facilisi cras fermentum odio eu. Tempus imperdiet nulla malesuada pellentesque elit eget gravida cum. Nulla facilisi cras fermentum odio eu. Sit amet dictum sit amet. Mauris cursus mattis molestie a iaculis. At volutpat diam ut venenatis tellus in metus vulputate. Suspendisse potenti nullam ac tortor vitae purus faucibus. Elit ullamcorper dignissim cras tincidunt lobortis. Nulla facilisi morbi tempus iaculis urna id.";
const string str9 = "Eu augue ut lectus arcu bibendum. Cras fermentum odio eu feugiat pretium nibh. Massa enim nec dui nunc. Sed odio morbi quis commodo. Montes nascetur ridiculus mus mauris vitae ultricies leo integer. Ullamcorper dignissim cras tincidunt lobortis feugiat vivamus at augue. Scelerisque eu ultrices vitae auctor eu augue. Nisi lacus sed viverra tellus. A pellentesque sit amet porttitor eget dolor morbi non arcu. Fermentum odio eu feugiat pretium. A cras semper auctor neque vitae tempus quam pellentesque. Varius vel pharetra vel turpis. Sapien eget mi proin sed libero enim. Et malesuada fames ac turpis egestas sed tempus urna. Et molestie ac feugiat sed lectus vestibulum mattis ullamcorper velit. Non pulvinar neque laoreet suspendisse. A diam sollicitudin tempor id eu nisl nunc.";

// ===== Name of the test archive
const string ArchiveName = "./TestArchive.zip";

// ===== Function Declarations
void CreateArchive();
void ModifyArchive();
void PrintArchiveLayout();
void PrintArchiveStats();
void PrintLayout(const Zippy::ZipArchive& arch, const std::string& folder = "", const std::string& indent = "");

int main() {

    // ===== Create archive and add items to it
    CreateArchive();

    // ===== Print archive layout
    PrintArchiveLayout();

    // ===== Print archive statistics
    PrintArchiveStats();

    return 0;
}

void CreateArchive() {
    // ===== Creating empty archive
    Zippy::ZipArchive arch;
    arch.Create(ArchiveName);

    // ===== Adding 20 entries to the archive
    //arch.AddEntry("Folder/","");
    arch.AddEntry("File 0.txt", str0);
    arch.AddEntry("File 1.txt", str1);
    arch.AddEntry("File 2.txt", str2);
    arch.AddEntry("File 3.txt", str3);
    arch.AddEntry("File 4.txt", str4);
    arch.AddEntry("File 5.txt", str5);
    arch.AddEntry("File 6.txt", str6);
    arch.AddEntry("File 7.txt", str7);
    arch.AddEntry("File 8.txt", str8);
    arch.AddEntry("File 9.txt", str9);
    arch.AddEntry("Folder0/Sub0/SubSub0/File 0.txt", str0);
    arch.AddEntry("Folder0/Sub0/File 1.txt", str1);
    arch.AddEntry("Folder2/File 2.txt", str2);
    arch.AddEntry("Folder3/File 3.txt", str3);
    arch.AddEntry("Folder4/File 4.txt", str4);
    arch.AddEntry("Folder5/File 5.txt", str5);
    arch.AddEntry("Folder6/File 6.txt", str6);
    arch.AddEntry("Folder7/File 7.txt", str7);
    arch.AddEntry("Folder8/File 8.txt", str8);
    arch.AddEntry("Folder9/File 9.txt", str9);

    Zippy::ZipEntryData data;
    for (auto ch : image)
        data.emplace_back(ch);

    arch.AddEntry("Image.png", data);

    // ===== Save and close the archive
    arch.Save();
    arch.Close();
}

void PrintLayout(const Zippy::ZipArchive& arch, const std::string& folder, const std::string& indent) {

    if (folder.empty())
        cout << "Root/" << endl;
    else
        if (std::count(folder.begin(), folder.end(), '/') <= 1)
            cout << indent << folder << endl;
        else
            cout << indent << folder.substr(folder.rfind('/', folder.size() - 2)) <<  endl;

    for (auto &file : arch.GetEntryNamesInDir(folder, false, true)) {
        if (std::count(file.begin(), file.end(), '/') == 0)
            cout << indent + "\t" << file << endl;
        else
            cout << indent + "\t" << file.substr(file.rfind('/') + 1) << endl;
    }

    for (auto &subfolder : arch.GetEntryNamesInDir(folder, true, false)) {
        PrintLayout(arch, subfolder, indent + "\t");
    }
}

void PrintArchiveLayout() {
    Zippy::ZipArchive arch;
    arch.Open(ArchiveName);

    PrintLayout(arch);
    cout << endl;
}

void PrintArchiveStats() {
    // ===== Open and check contents
    Zippy::ZipArchive arch;
    arch.Open(ArchiveName);

    TablePrinter::TablePrinter tp(&std::cout);
    tp.AddColumn("Index", 6);
    tp.set_flush_left();
    tp.AddColumn("File", 40);
    tp.set_flush_right();
    tp.AddColumn("Uncompressed Size", 18);
    tp.AddColumn("Compressed Size", 16);
    tp.AddColumn("Dir", 5);
    tp.AddColumn("Time Stamp", 20);

    tp.PrintHeader();

    for (auto &name : arch.GetEntryNames()) {
        //string name = item;
        auto entry = arch.GetEntry(name);
        char mbstr[100];
        std::strftime(mbstr, sizeof(mbstr), "%F %T", std::localtime(&entry.Time()));
        string str = mbstr;

        tp << entry.Index();
        tp.set_flush_left();
        tp << entry.Filename();
        tp.set_flush_right();
        tp << entry.UncompressedSize()
           << entry.CompressedSize()
           << (entry.IsDirectory() ? "Y" : "N")
           << str;
    }

    tp.PrintFooter();

    arch.ExtractEntry("Image.png", "./");

    arch.Close();
}