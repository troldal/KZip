/*
    MIT License

    Copyright (c) 2019 Kenneth Troldal Balslev

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

 */

#ifndef Zippy_ZIPARCHIVE_H
#define Zippy_ZIPARCHIVE_H

#include <string>
#include <vector>

#include "miniz/miniz.h"
#include "ZipEntry.h"

namespace Zippy {

    /**
     * @brief The ZipArchive class represents the zip archive file as a whole. It consists of the individual zip entries, which
     * can be both files and folders.
     */
    class ZipArchive {

    public:

        /**
         * @brief Constructor. Constructs a null-archive, which can be used for creating a new .zip file
         * or opening an existing one.
         */
        explicit ZipArchive() = default;

        /**
         * @brief Constructor. Constructs an archive object, using the fileName input parameter. If the file already exists,
         * it will be opened. Otherwise, a new object will be created.
         * @param fileName The name of the file to open or create.
         */
        explicit ZipArchive(const std::string& fileName);

        /**
         * @brief Copy Constructor (deleted).
         * @param other The object to copy
         */
        ZipArchive(const ZipArchive& other) = delete;

        /**
         * @brief Move Constructor.
         * @param other The object to be moved.
         */
        ZipArchive(ZipArchive&& other) = default;

        /**
         * @brief Destructor.
         */
        virtual ~ZipArchive();

        /**
         * @brief Copy Assignment Operator (deleted)
         * @param other The object to be copied.
         * @return A reference to the copied-to object.
         */
        ZipArchive& operator=(const ZipArchive& other) = delete;

        /**
         * @brief Move Assignment Operator.
         * @param other The object to be moved.
         * @return A reference to the moved-to object.
         */
        ZipArchive& operator=(ZipArchive&& other) = default;

        /**
         * @brief Creates a new (empty) archive file with the given filename.
         * @param fileName The filename for the new archive.
         */
        void Create(const std::string& fileName);

        /**
         * @brief Open an existing archive file with the given filename.
         * @param fileName The filename of the archive to open.
         */
        void Open(const std::string& fileName);

        /**
         * @brief Close the archive for reading and writing.
         */
        void Close();

        /**
         * @brief Checks if the archive file is open for reading and writing.
         * @return true if it is open; otherwise false;
         */
        bool IsOpen();

        /**
         * @brief
         * @param includeDirs
         * @param includeFiles
         * @return
         */
        std::vector<std::string> GetEntryNames(bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief
         * @param dir
         * @param includeDirs
         * @param includeFiles
         * @return
         */
        std::vector<std::string> GetEntryNamesInDir(const std::string& dir, bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief
         * @param includeDirs
         * @param includeFiles
         * @return
         */
        std::vector<ZipEntryMetaData> GetMetaData(bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief
         * @param dir
         * @param includeDirs
         * @param includeFiles
         * @return
         */
        std::vector<ZipEntryMetaData> GetMetaDataInDir(const std::string& dir, bool includeDirs = true, bool includeFiles = true);
        /**
         * @brief
         * @param includeDirs
         * @param includeFiles
         * @return
         */
        int GetNumEntries(bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief
         * @param dir
         * @param includeDirs
         * @param includeFiles
         * @return
         */
        int GetNumEntriesInDir(const std::string& dir, bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief
         * @param entryName
         * @param includeDirs
         * @param includeFiles
         * @return
         */
        bool HasEntry(const std::string& entryName, bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief
         */
        void Save();

        /**
         * @brief
         * @param filename
         */
        void SaveAs(std::string filename);

        /**
         * @brief
         * @param name
         */
        void DeleteEntry(const std::string& name);

        /**
         * @brief
         * @param name
         * @return
         */
        ZipEntry GetEntry(const std::string& name);

        /**
         * @brief
         * @param name
         * @param data
         * @return
         */
        ZipEntry AddEntry(const std::string& name, const ZipEntryData& data);

        /**
         * @brief
         * @param name
         * @param data
         * @return
         */
        ZipEntry AddEntry(const std::string& name, const std::string& data);

    private:

        /**
         * @brief
         * @param error
         * @param errorString
         */
        static void ThrowException(mz_zip_error error, const std::string& errorString);

        /**
         * @brief
         * @return
         */
        static std::string GenerateRandomName();

    private:
        mz_zip_archive m_Archive     = mz_zip_archive(); /**< */
        std::string    m_ArchivePath = ""; /**< */
        bool           m_IsOpen      = false; /**< */

        std::vector<Impl::ZipEntry> m_ZipEntries = std::vector<Impl::ZipEntry>(); /**< */

    };
}  // namespace Zippy

#endif //Zippy_ZIPARCHIVE_H
