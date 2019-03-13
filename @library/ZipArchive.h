//
// Created by Troldal on 2019-03-10.
//

#ifndef MINIZ_ZIPARCHIVE_H
#define MINIZ_ZIPARCHIVE_H

#include <string>
#include <vector>

#include "miniz/miniz.h"
#include "ZipEntry.h"

namespace SimpleZip {

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
        ZipEntry& GetEntry(const std::string& name);

        /**
         * @brief
         * @param name
         * @param data
         * @return
         */
        ZipEntry& AddEntry(const std::string& name, const ZipEntryData& data);

        /**
         * @brief
         * @param name
         * @param data
         * @return
         */
        ZipEntry& AddEntry(const std::string& name, const std::string& data);

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

        std::vector<ZipEntry> m_ZipEntries; /**< */



    };

}

#endif //MINIZ_ZIPARCHIVE_H
