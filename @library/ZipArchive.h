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

#ifndef SIMPLEZIP_ZIPARCHIVE_H
#define SIMPLEZIP_ZIPARCHIVE_H

#include <string>
#include <vector>

#include "miniz/miniz.h"
#include "ZipEntry.h"

namespace SimpleZip {

    /**
     * @brief
     */
    class ZipArchive {

    public:

        /**
         * @brief
         */
        explicit ZipArchive() = default;

        /**
         * @brief
         * @param fileName
         */
        explicit ZipArchive(const std::string& fileName);

        /**
         * @brief
         * @param other
         */
        ZipArchive(const ZipArchive& other) = delete;

        /**
         * @brief
         * @param other
         */
        ZipArchive(ZipArchive&& other) = default;

        /**
         * @brief
         */
        virtual ~ZipArchive();

        /**
         * @brief
         * @param other
         * @return
         */
        ZipArchive& operator=(const ZipArchive& other) = delete;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipArchive& operator=(ZipArchive&& other) = default;

        /**
         * @brief
         * @param fileName
         */
        void Create(const std::string& fileName);

        /**
         * @brief
         * @param fileName
         */
        void Open(const std::string& fileName);

        /**
         * @brief
         */
        void Close();

        /**
         * @brief
         * @return
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
        mz_zip_archive m_Archive = mz_zip_archive(); /**< */
        std::string    m_ArchivePath = ""; /**< */
        bool           m_IsOpen      = false; /**< */

        std::vector<Impl::ZipEntry> m_ZipEntries = std::vector<Impl::ZipEntry>(); /**< */

    };
}  // namespace SimpleZip

#endif //SIMPLEZIP_ZIPARCHIVE_H
