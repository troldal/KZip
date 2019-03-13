//
// Created by Troldal on 2019-03-11.
//

#ifndef MINIZ_ZIPENTRY_H
#define MINIZ_ZIPENTRY_H

// ===== Standard Library Includes
#include <string>
#include <vector>

// ===== Other Includes
#include "miniz/miniz.h"

namespace SimpleZip {

    // ===== Alias Declarations
    using ZipEntryInfo = mz_zip_archive_file_stat;
    using ZipEntryData = std::vector<std::byte>;

    /**
     * @brief
     */
    class ZipEntry {
        friend class ZipArchive;

    public:

        // ===== Constructors, Destructor and Operators

        /**
         * @brief Constructor. Creates a new ZipEntry with the given ZipEntryInfo parameter. This is only used for creating
         * a ZipEntry for an entry already present in the ZipArchive.
         * @param info
         */
        explicit ZipEntry(const ZipEntryInfo& info);

        /**
         * @brief Constructor. Creates a new ZipEntry with the given name and binary data. This should only be used for creating
         * new entries, not already present in the ZipArchive
         * @param name
         * @param data
         */
        ZipEntry(const std::string& name, const ZipEntryData& data);

        /**
         * @brief Constructor. Creates a new ZipEntry with the given name and string data. This should only be used for creating
         * new entries, not already present in the ZipArchive
         * @param name
         * @param data
         */
        ZipEntry(const std::string& name, const std::string& data);

        /**
         * @brief
         * @param other
         */
        ZipEntry(const ZipEntry& other) = delete;

        /**
         * @brief
         * @param other
         */
        ZipEntry(ZipEntry&& other) noexcept = default;

        /**
         * @brief
         */
        virtual ~ZipEntry() = default;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntry& operator=(const ZipEntry& other) = delete;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntry& operator=(ZipEntry&& other) noexcept = default;


        // ===== Data Access and Manipulation

        /**
         * @brief
         * @return
         */
        ZipEntryData GetData() const;

        /**
         * @brief
         * @return
         */
        std::string GetDataAsString() const;

        /**
         * @brief
         * @param data
         */
        void SetData(const std::string& data);


        // ===== Metadata Access

        /**
         * @brief
         * @return
         */
        uint32_t Index() const;

        /**
         * @brief
         * @return
         */
        uint64_t CompressedSize() const;

        /**
         * @brief
         * @return
         */
        uint64_t UncompressedSize() const;

        /**
         * @brief
         * @return
         */
        bool IsDirectory() const;

        /**
         * @brief
         * @return
         */
        bool IsEncrypted() const;

        /**
         * @brief
         * @return
         */
        bool IsSupported() const;

        /**
         * @brief
         * @return
         */
        std::string Filename() const;

        /**
         * @brief
         * @return
         */
        std::string Comment() const;

        /**
         * @brief
         * @return
         */
        const time_t& Time() const;

    private:
        ZipEntryInfo m_EntryInfo = ZipEntryInfo(); /**< */
        ZipEntryData m_EntryData = ZipEntryData(); /**< */

        bool m_IsModified = false; /**< */

        /**
         * @brief
         * @return
         */
        bool IsModified() const;

        static uint32_t s_LatestIndex;
        static uint32_t GetNewIndex();
        static ZipEntryInfo CreateInfo(const std::string& name);

    };
}  // namespace SimpleZip

#endif //MINIZ_ZIPENTRY_H
