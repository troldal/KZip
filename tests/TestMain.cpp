//
// Created by Troldal on 2019-01-10.
//

// this tells catch to provide a main()
// only do this in one cpp file

#ifndef TESTMAIN_H
#define TESTMAIN_H

#define CATCH_CONFIG_RUNNER

#include <catch.hpp>
#include <iostream>
#include <fstream>

#include "test-data-winzip-archive.hpp"

void CreateWinZipFile(const std::string& filename);

int main(int argc, char* argv[]) {

    // Global Setup
    std::cout << std::endl;
    std::cout << "Creating .zip file made with WinZip..." << std::endl;
    const std::string filename = "./CreatedWithWinZip.zip";
    CreateWinZipFile(filename);
    // Global Setup Complete

    // Run Test Suite
    std::cout << "Running test suite..." << std::endl;
    int result = Catch::Session().run(argc, argv);
    // Run Test Suite Complete

    // Global Clean-up
    std::cout << "Cleaning up..." << std::endl;
    remove(filename.c_str());
    // Global Clean-up Complete

    std::cout << "Test complete!" << std::endl;
    return result;
}

// This is a binary representation of a .zip file created with WinZip.
void CreateWinZipFile(const std::string& filename) {

    std::ofstream outfile(filename, std::ios::binary);
    outfile.write(reinterpret_cast<char const*>(winzipdata.data()), winzipdata.size());
    outfile.close();
}

#endif //TESTMAIN_H