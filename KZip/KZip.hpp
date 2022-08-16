#ifndef KZIP_HPP
#define KZIP_HPP

#pragma warning(push)
#pragma warning(disable : 4334)
#pragma warning(disable : 4996)
#pragma warning(disable : 4127)
#pragma warning(disable : 4267)
#pragma warning(disable : 4100)
#pragma warning(disable : 4245)
#pragma warning(disable : 4267)
#pragma warning(disable : 4242)
#pragma warning(disable : 4244)

// ===== External Includes =====
#include "deps/miniz.h"

// ===== Standard Includes =====
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// TODO(troldal): Is this required???
#ifdef _WIN32
#    include <direct.h>
#endif

namespace KZip
{

    namespace fs = std::filesystem;

    /**
     * @brief The ZipRuntimeError class is a custom exception class derived from the std::runtime_error class.
     * @details In case of an error in the KZip KZip, a ZipRuntimeError object will be thrown, with a message
     * describing the details of the error.
     */
    class ZipRuntimeError : public std::runtime_error
    {
    public:
        /**
         * @brief Constructor.
         * @param err A string with a description of the error.
         */
        inline explicit ZipRuntimeError(const std::string& err) : runtime_error(err) {}
    };

    /**
     * @brief The ZipLogicError class is a custom exception class derived from the std::logic_error class.
     * @details In case of an error in the input, a ZipLogicError object will be thrown, with a message
     * describing the details of the error.
     */
    class ZipLogicError : public std::logic_error
    {
    public:
        /**
         * @brief
         * @param err
         */
        inline explicit ZipLogicError(const std::string& err) : logic_error(err) {}
    };

}    // namespace KZip

namespace KZip
{
    namespace Impl
    {
        class ZipEntryWrapper;
        class ZipArchive;
    }    // namespace Impl

    class ZipArchive;

    /**
     * @brief The ZipEntryData entity is an alias for a std::vector of std::bytes.
     * @details This is used as a generic container of file data of any kind, both character strings and binary.
     * A vector of char or an array of char can also be used, but a vector of bytes makes it clearer that it can
     * also be used for non-text data.
     */
    using ZipEntryData = std::vector<unsigned char>;

    /**
     * @brief
     */
    enum class ZipFlags : uint8_t { Files = 1, Directories = 2 };

    /**
     * @brief
     * @param first
     * @param second
     * @return
     */
    inline ZipFlags operator|(ZipFlags first, ZipFlags second)
    {
        return static_cast<ZipFlags>(static_cast<uint8_t>(first) | static_cast<uint8_t>(second));
    }

    /**
     * @brief
     * @param first
     * @param second
     * @return
     */
    inline ZipFlags operator&(ZipFlags first, ZipFlags second)
    {
        return static_cast<ZipFlags>(static_cast<uint8_t>(first) & static_cast<uint8_t>(second));
    }

    /**
     * @brief The ZipEntryMetaData is essentially a wrapper around the ZipEntryInfo struct, which is an alias for a
     * miniz struct.
     */
    class ZipEntryMetaData
    {
    public:
        /**
         * @brief Constructor.
         * @param info A reference to a ZipEntryInfo object.
         */
        explicit ZipEntryMetaData(const mz_zip_archive_file_stat& info) : m_stats(info) {}

        uint32_t         index() const { return m_stats.m_file_index; }
        uint64_t         compressedSize() const { return m_stats.m_comp_size; }
        uint64_t         uncompressedSize() const { return m_stats.m_uncomp_size; }
        bool             isDirectory() const { return m_stats.m_is_directory; }
        bool             isEncrypted() const { return m_stats.m_is_encrypted; }
        bool             isSupported() const { return m_stats.m_is_supported; }
        std::string_view name() const { return m_stats.m_filename; }
        std::string_view comment() const { return m_stats.m_comment; }
        time_t           time() const { return m_stats.m_time; }

    private:
        mz_zip_archive_file_stat m_stats;
    };

    /**
     *
     */
    class ZipEntry
    {
        friend class ZipEntryProxy;
    public:

        /**
         * @brief
         * @param filename
         */
        explicit ZipEntry(const std::string& filename) {
            m_info.m_file_index       = 0;
            m_info.m_central_dir_ofs  = 0;
            m_info.m_version_made_by  = 0;
            m_info.m_version_needed   = 0;
            m_info.m_bit_flag         = 0;
            m_info.m_method           = 0;
            m_info.m_time             = std::time(nullptr);
            m_info.m_crc32            = 0;
            m_info.m_comp_size        = 0;
            m_info.m_uncomp_size      = 0;
            m_info.m_internal_attr    = 0;
            m_info.m_external_attr    = 0;
            m_info.m_local_header_ofs = 0;
            m_info.m_comment_size     = 0;
            m_info.m_is_directory     = (filename.back() == '/');
            m_info.m_is_encrypted     = false;
            m_info.m_is_supported     = true;

#if _MSC_VER    // On MSVC, use the safe version of strcpy
            strcpy_s(m_info.m_filename, sizeof m_info.m_filename, filename.c_str());
            strcpy_s(m_info.m_comment, sizeof m_info.m_comment, "");
#else    // Otherwise, use the unsafe version as fallback :(
            strncpy(m_info.m_filename, filename.c_str(), sizeof m_info.m_filename);    // NOLINT
            strncpy(m_info.m_comment, "", sizeof m_info.m_comment);                // NOLINT
#endif
        }

        /**
         * @brief
         * @param other
         */
        ZipEntry(const ZipEntry& other) = default;

        /**
         * @brief
         * @param other
         */
        ZipEntry(ZipEntry&& other) noexcept = default;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntry& operator=(const ZipEntry& other) = default;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntry& operator=(ZipEntry&& other) noexcept = default;

        /**
         * @brief
         */
        ~ZipEntry() = default;

        /**
         * @brief
         * @tparam T
         * @param data
         * @return
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<typename T::value_type, unsigned char>>::type* = nullptr>
        ZipEntry& operator=(T data)
        {
            setData(data);
            return *this;
        }

        /**
         * @brief
         * @tparam T
         * @param data
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<typename T::value_type, unsigned char>>::type* = nullptr>
        void setData(T data)
        {
            if constexpr (std::is_same_v<T, std::vector<unsigned char> >)
                m_data = data;

            else {
                m_data = std::vector<unsigned char> {data.begin(), data.end()};
            }
        }

        /**
         * @brief Function for extracting the zip data to any compatible container.
         * @details This templated getter allows extraction of the zip data to any container
         * that holds a type convertible from unsigned char, and that can be constructed from begin and end
         * iterators from a source.
         * @note While any iterator-based container with an unsigned char value type can be used,
         * the function is optimized for std::string and std::vector<unsigned char>
         * @tparam T The type to convert to (e.g. std::string or std::vector<unsigned char>
         * @return An object of type T, holding the zip data.
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<typename T::value_type, unsigned char>>::type* = nullptr>
        T getData() const
        {
            if (!m_data.has_value()) return {};
            return {m_data->begin(), m_data->end()};
        }

        /**
         * @brief Implicit type conversion operator.
         * @details This templated type conversion operator allows extraction of the zip data to any container
         * that holds a type convertible from unsigned char, and that can be constructed from begin and end
         * iterators from a source. This function simply calls the getData<T>() function.
         * @tparam T The type to convert to (e.g. std::string or std::vector<unsigned char>
         * @return An object of type T, holding the zip data.
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<unsigned char, typename T::value_type>>::type* = nullptr>
        operator T() const // NOLINT
        {    // NOLINT
            return getData<T>();
        }

        /**
         * @brief
         */
        void erase()
        {
            if(m_data) m_data.reset();
        }

        /**
         * @brief
         * @return
         */
        std::string_view name() const {
            return {m_info.m_filename};
        }

        /**
         * @brief
         * @param filename
         */
        void setName(const std::string& entryname) {
            if (entryname.empty()) throw ZipLogicError("Entry name must not be empty");

#if _MSC_VER    // On MSVC, use the safe version of strcpy
            strcpy_s(m_info.m_filename, sizeof m_info.m_filename, entryname.c_str());
            strcpy_s(m_info.m_comment, sizeof m_info.m_comment, "");
#else    // Otherwise, use the unsafe version as fallback :(
            strncpy(m_info.m_filename, entryname.c_str(), sizeof m_info.m_filename);    // NOLINT
            strncpy(m_info.m_comment, "", sizeof m_info.m_comment);                // NOLINT
#endif
        }

        /**
         * @brief
         * @return
         */
        ZipEntryMetaData metadata() const { return ZipEntryMetaData(m_info); }

        /**
         * @brief
         * @tparam T
         * @param other
         * @return
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<unsigned char, typename T::value_type> ||
                                                     std::is_same_v<std::decay_t<T>, const char*> ||
                                                     std::is_same_v<std::decay_t<T>, char*> >::type* = nullptr>
        bool operator==(const T& other) const // NOLINT
        {
            auto data = getData<T>();
            auto result = std::mismatch(other.begin(), other.end(), data.begin());
            return (result.first == other.end() || result.second == data.end());
        }

    private:

        /**
         * @brief
         * @param archive
         * @param info
         */
        ZipEntry(std::vector<unsigned char> data, mz_zip_archive_file_stat info) : m_data(data), m_info(info) {}

        //---------- Private Member Variables ---------- //
        mz_zip_archive_file_stat m_info    = mz_zip_archive_file_stat(); /**< File stats for the entry. */
        std::optional<std::vector<unsigned char> > m_data = {};
    };

    class ZipEntryProxy {
        friend class Impl::ZipArchive;
        friend class Impl::ZipEntryWrapper;
    public:

        /**
         * @brief
         */
        ~ZipEntryProxy() = default;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntryProxy& operator=(const ZipEntryProxy& other) {
            if (&other != this) {
                *this = other.getData<std::vector<unsigned char>>();
            }

            return *this;
        };

        /**
         * @brief
         * @tparam T
         * @param data
         * @return
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<typename T::value_type, unsigned char>>::type* = nullptr>
        ZipEntryProxy& operator=(T data)
        {
            setData(data);
            return *this;
        }

        /**
         * @brief
         * @tparam T
         * @param data
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<typename T::value_type, unsigned char>>::type* = nullptr>
        void setData(T data)
        {
            if constexpr (std::is_same_v<T, std::vector<unsigned char> >)
                m_data = data;

            else {
                m_data = std::vector<unsigned char> {data.begin(), data.end()};
            }
        }

        /**
         * @brief Function for extracting the zip data to any compatible container.
         * @details This templated getter allows extraction of the zip data to any container
         * that holds a type convertible from unsigned char, and that can be constructed from begin and end
         * iterators from a source.
         * @note While any iterator-based container with an unsigned char value type can be used,
         * the function is optimized for std::string and std::vector<unsigned char>
         * @tparam T The type to convert to (e.g. std::string or std::vector<unsigned char>
         * @return An object of type T, holding the zip data.
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<typename T::value_type, unsigned char>>::type* = nullptr>
        T getData() const
        {
            if (m_data.has_value())
                return {m_data->begin(), m_data->end()};

            // ===== Optimization for std::string
            if constexpr (std::is_same_v<T, std::string>) {

                // ===== Create a temporary vector of unsinged char, to hold the zip data
                std::string data;
                data.resize(m_info.m_uncomp_size);
                mz_zip_reader_extract_to_mem(m_archive, m_info.m_file_index, data.data(), m_info.m_uncomp_size, 0);

                // ===== Check that the operation was successful
                if (!m_info.m_is_directory && data.size() != m_info.m_uncomp_size) {
                    throw ZipRuntimeError(mz_zip_get_error_string(m_archive->m_last_error));
                }

                return data;
            }

            // ===== Optimization for std::vector<unsigned char>
            else if constexpr (std::is_same_v<T, std::vector<unsigned char>>) {

                // ===== Create a temporary vector of unsinged char, to hold the zip data
                std::vector<unsigned char> data;
                data.resize(m_info.m_uncomp_size);
                mz_zip_reader_extract_to_mem(m_archive, m_info.m_file_index, data.data(), m_info.m_uncomp_size, 0);

                // ===== Check that the operation was successful
                if (!m_info.m_is_directory && data.size() != m_info.m_uncomp_size) {
                    throw ZipRuntimeError(mz_zip_get_error_string(m_archive->m_last_error));
                }

                return data;
            }

                // ===== General case
            else {
                // ===== Create a temporary vector of unsinged char, to hold the zip data
                std::vector<unsigned char> data;
                data.resize(m_info.m_uncomp_size);
                mz_zip_reader_extract_to_mem(m_archive, m_info.m_file_index, data.data(), m_info.m_uncomp_size, 0);

                // ===== Check that the operation was successful
                if (!m_info.m_is_directory && data.size() != m_info.m_uncomp_size) {
                    throw ZipRuntimeError(mz_zip_get_error_string(m_archive->m_last_error));
                }

                return { data.begin(), data.end() };
            }
        }

        /**
         * @brief Implicit type conversion operator.
         * @details This templated type conversion operator allows extraction of the zip data to any container
         * that holds a type convertible from unsigned char, and that can be constructed from begin and end
         * iterators from a source. This function simply calls the getData<T>() function.
         * @tparam T The type to convert to (e.g. std::string or std::vector<unsigned char>
         * @return An object of type T, holding the zip data.
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<unsigned char, typename T::value_type>>::type* = nullptr>
        operator T() const // NOLINT
        {
            return getData<T>();
        }

        operator ZipEntry() { // NOLINT
            return {getData<std::vector<unsigned char>>(), m_info};
        }

        /**
         * @brief
         */
        void clear()
        {
            // TODO(troldal): To be implemented
        }

        std::string_view name() const {
            return {m_info.m_filename};
        }

        /**
         * @brief
         * @return
         */
        ZipEntryMetaData metadata() const { return ZipEntryMetaData(m_info); }

        template<typename T, typename std::enable_if<std::is_convertible_v<unsigned char, typename T::value_type> ||
                                                     std::is_same_v<std::decay_t<T>, const char*> ||
                                                     std::is_same_v<std::decay_t<T>, char*> >::type* = nullptr>
        bool operator==(const T& other) const // NOLINT
        {
            auto data = getData<T>();
            auto result = std::mismatch(other.begin(), other.end(), data.begin());
            return (result.first == other.end() || result.second == data.end());
        }

    private:
        //---------- Private Member Functions ---------- //

        /**
         * @brief
         * @param archive
         * @param info
         */
        ZipEntryProxy(mz_zip_archive* archive, mz_zip_archive_file_stat info) : m_archive(archive), m_info(info) {}

        /**
         * @brief
         * @param other
         */
        ZipEntryProxy(const ZipEntryProxy& other) = default;

        /**
         * @brief
         * @param other
         */
        ZipEntryProxy(ZipEntryProxy&& other) noexcept = default;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntryProxy& operator=(ZipEntryProxy&& other) noexcept = default;

        /**
         *
         * @return
         */
        const mz_zip_archive_file_stat& stats() const {
            return m_info;
        }

        /**
         *
         * @return
         */
        bool isUpdated() const {
            return m_data.has_value();
        }

        /**
         * @brief
         * @return
         */
        uint64_t size() const {
            if (isUpdated())
                return m_data.has_value() ? m_data.value().size() : 0;

            return m_info.m_uncomp_size;
        }

        /**
         * @brief
         * @return
         */
        const std::vector<unsigned char>& rawData() const {
            return m_data.value();
        }

        //---------- Private Member Variables ---------- //
        mz_zip_archive*          m_archive = nullptr;
        mz_zip_archive_file_stat m_info    = mz_zip_archive_file_stat(); /**< File stats for the entry. */
        std::optional<std::vector<unsigned char> > m_data = {};
    }; // ZipEntryProxy



    namespace Impl
    {
        /**
         * @brief
         */
        class ZipEntryWrapper {
        public:

            /**
             * @brief
             * @param entry
             */
            explicit ZipEntryWrapper(const ZipEntryProxy& entry) : m_entry(entry) {}

            /**
             * @brief
             * @return
             */
            const ZipEntryProxy& entry() const {
                return m_entry;
            }

            /**
             * @brief
             * @return
             */
            ZipEntryProxy& entry() {
                return m_entry;
            }

        private:
            ZipEntryProxy m_entry;
        };

        /**
         * @brief
         */
        class ZipArchive
        {
            friend class KZip::ZipArchive;

        public:
            /**
             * @brief
             */
            ZipArchive() = default;

            /**
             * @brief Constructor. Constructs an archive object, using the fileName input parameter. If the file already exists,
             * it will be opened. Otherwise, a new object will be created.
             * @param fileName The name/path of the file to open or create.
             */
            explicit ZipArchive(const fs::path& fileName)
            {
                // ===== If successful, continue to open the file.
                if (fs::exists(fileName)) open(fileName);

                // ===== If unsuccessful, create the archive file and continue.
                else
                    create(fileName);
            }

            /**
             * @brief
             * @param other
             */
            ZipArchive(const ZipArchive& other) = delete;

            /**
             * @brief
             * @param other
             */
            ZipArchive(ZipArchive&& other) noexcept = default;

            /**
             * @brief
             */
            ~ZipArchive() { close(); }

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
             * @brief Creates a new (empty) archive file with the given filename.
             * @details A new archive file is created and initialized and thereafter closed, creating an empty archive file.
             * After ensuring that the file is valid (i.e. not somehow corrupted), it is opened using the open() member function.
             * @param fileName The filename/path for the new archive.
             */
            inline void create(const fs::path& fileName)
            {
                // ===== Prepare an archive file;
                mz_zip_archive archive = mz_zip_archive();
                mz_zip_writer_init_file(&archive, fileName.string().c_str(), 0);

                // ===== Finalize and close the temporary archive
                mz_zip_writer_finalize_archive(&archive);
                mz_zip_writer_end(&archive);

                // ===== Validate the temporary file
                mz_zip_error errordata { mz_zip_error() };
                if (!mz_zip_validate_file_archive(fileName.string().c_str(), 0, &errordata)) {
                    throw ZipRuntimeError(mz_zip_get_error_string(errordata));
                }

                // ===== If everything is OK, open the newly created archive.
                open(fileName);
            }

            /**
             * @brief Open an existing archive file with the given filename.
             * @details The archive file is opened and meta data for all the entries in the archive is loaded into memory.
             * @param fileName The filename/path of the archive to open.
             * @note If more than one entry with the same name exists in the archive, only the newest one will be loaded.
             * When saving the archive, only the loaded entries will be kept; other entries with the same name will be deleted.
             */
            inline void open(const fs::path& fileName)
            {
                if (isOpen()) close();

                // ===== Open the zip archive file. If unsuccessful, throw exception.
                m_archivePath = fileName;
                if (!mz_zip_reader_init_file(&m_archive, m_archivePath.string().c_str(), 0)) {
                    throw ZipRuntimeError(mz_zip_get_error_string(m_archive.m_last_error));
                }
                m_isOpen = true;

                // ===== Iterate through the archive and add the entries to the internal data structure
                mz_zip_archive_file_stat info;
                for (unsigned int i = 0; i < mz_zip_reader_get_num_files(&m_archive); ++i) {
                    if (!mz_zip_reader_file_stat(&m_archive, i, &info)) {
                        throw ZipRuntimeError(mz_zip_get_error_string(m_archive.m_last_error));
                    }

                    if (info.m_file_index > m_currentIndex) m_currentIndex = info.m_file_index;
                    m_zipEntryData.emplace_back(ZipEntryProxy(&m_archive, info));
                }

                // ===== Remove entries with identical names. The newest entries will be retained.
                // TODO (troldal): Can this be done without reversing the list twice?
                auto isEqual = [](const ZipEntryWrapper& a, const ZipEntryWrapper& b) {
                    return strcmp(a.entry().stats().m_filename, b.entry().stats().m_filename) == 0;
                };
                std::reverse(m_zipEntryData.begin(), m_zipEntryData.end());
                m_zipEntryData.erase(std::unique(m_zipEntryData.begin(), m_zipEntryData.end(), isEqual), m_zipEntryData.end());
                std::reverse(m_zipEntryData.begin(), m_zipEntryData.end());

                // ===== Add folder entries if they don't exist
                for (auto& entry : entryNames(ZipFlags::Directories)) {
                    if (entry.find('/') != std::string::npos) {
                        addEntry(entry.substr(0, entry.rfind('/') + 1));
                    }
                }
            }

            /**
             * @brief Add an entry to the archive.
             * @details If an entry with the given name/path exists, it will be overwritten. Otherwise, a new entry will be created.
             * @note Any folders in the path that does not exist, will be created as separate entries.
             * @param path
             * @param data
             */
            inline ZipEntryProxy& addEntry(const std::string& path)
            {
                if (!isOpen()) throw ZipLogicError("Function call: addEntry(). Archive is invalid or not open!");

                // ===== Check if an entry with the given name already exists in the archive.
                auto files  = entryNames(ZipFlags::Files);
                auto result = std::find_if(m_zipEntryData.begin(), m_zipEntryData.end(), [&](const ZipEntryWrapper& item) {
                    return strcmp(item.entry().stats().m_filename, path.c_str()) == 0;    // NOLINT
                });

//                if (result != m_zipEntryData.end()) throw ZipLogicError("KZip Error: Entry '" + path + "' already exists");

                // ===== Ensure that all folders and subfolders in the path name have an entry in the archive
                auto folders  = entryNames(ZipFlags::Directories);
                auto position = uint64_t { 0 };
                while (path.find('/', position) != std::string::npos) {
                    position        = path.find('/', position) + 1;
                    auto folderName = path.substr(0, position);

                    // ===== If folderName isn't registered in the archive, add it.
                    if (std::find(folders.begin(), folders.end(), folderName) == folders.end()) {
                        m_zipEntryData.emplace_back(ZipEntryProxy(&m_archive, createInfo(folderName)));
                    }
                }

                // ===== Create a new entry and return the reference
                if (result != m_zipEntryData.end()) {
                    *result = ZipEntryWrapper(ZipEntryProxy(&m_archive, createInfo(path)));
                    return result->entry();
                }
                return m_zipEntryData.emplace_back(ZipEntryProxy(&m_archive, createInfo(path))).entry();
            }

            /**
             * @brief
             * @param name
             */
            inline void deleteEntry(const std::string& name)
            {
                if (!isOpen()) throw ZipLogicError("Function call: deleteEntry(). Archive is invalid or not open!");

                // ===== When saving, only the entries present in the vector will be saved or copied from the original file.
                m_zipEntryData.erase(std::remove_if(m_zipEntryData.begin(),
                                                    m_zipEntryData.end(),
                                                    [&](const ZipEntryWrapper& entry) { return strcmp(name.c_str(), entry.entry().stats().m_filename) == 0; }),
                                     m_zipEntryData.end());
            }

            /**
             * @brief
             * @param path
             * @return
             */
            inline ZipEntryProxy& entry(const std::string& path)
            {
                if (!isOpen()) throw ZipLogicError("Cannot get entry from empty ZipArchive object!");

                // ===== Look up file_stat object.
                auto stats = std::find_if(m_zipEntryData.begin(), m_zipEntryData.end(), [&](const ZipEntryWrapper& data) {
                    return strcmp(path.c_str(), data.entry().stats().m_filename) == 0;
                });

                if (stats == m_zipEntryData.end()) throw ZipLogicError("KZip Error: Entry '" + path + "' does not exist");

                return stats->entry();
            }

            /**
             * @brief
             * @param path
             * @return
             */
            inline const ZipEntryProxy& entry(const std::string& path) const
            {
                if (!isOpen()) throw ZipLogicError("Cannot get entry from empty ZipArchive object!");

                // ===== Look up file_stat object.
                auto stats = std::find_if(m_zipEntryData.begin(), m_zipEntryData.end(), [&](const ZipEntryWrapper& data) {
                    return strcmp(path.c_str(), data.entry().stats().m_filename) == 0;
                });

                if (stats == m_zipEntryData.end()) throw ZipLogicError("KZip Error: Entry '" + path + "' does not exist");

                return stats->entry();
            }

//            inline void extract(const fs::path& destination, const std::string& entryName) {
//
//            }



            /**
             * @brief Get a list of the entries in the archive. Depending on the input parameters, the list will include
             * directories, files or both.
             * @param flags A ZipFlags enum indicating whether files and/or directories should be included. By default, only files are
             * included.
             * @return A std::vector of std::strings with the entry names.
             */
            inline std::vector<std::string> entryNames(ZipFlags flags) const
            {
                return entryNames("", flags);
            }

            /**
             * @brief
             * @param path
             * @param flags
             * @return
             */
            inline std::vector<std::string> entryNames(const std::string& path, ZipFlags flags) const
            {
                if (!isOpen()) throw ZipLogicError("Function call: entryNames(). Archive is invalid or not open!");

                std::vector<std::string> result;

                // ===== Iterate through all the entries in the archive
                for (const auto& item : m_zipEntryData) {
                    // ===== If directories should be included and the current entry is a directory, add it to the result.
                    if (path.empty() ||
                        (strlen(item.entry().stats().m_filename) >= path.size() &&
                         std::string_view(item.entry().stats().m_filename, path.size()) == path))
                    {
                        if (static_cast<bool>(flags & ZipFlags::Directories) && item.entry().stats().m_is_directory) {
                            result.emplace_back(item.entry().stats().m_filename);
                            continue;
                        }

                        // ===== If files should be included and the current entry is a file, add it to the result.
                        if (static_cast<bool>(flags & ZipFlags::Files) && !item.entry().stats().m_is_directory) {
                            result.emplace_back(item.entry().stats().m_filename);
                            continue;
                        }
                    }
                }

                return result;
            }

            /**
             * @brief
             * @param flags
             * @return
             */
            inline uint16_t entryCount(ZipFlags flags = (ZipFlags::Files)) const {
                return entryCount("", flags);
            }

            /**
             * @brief
             * @param path
             * @param flags
             * @return
             */
            inline uint16_t entryCount(const std::string& path, ZipFlags flags = (ZipFlags::Files)) const {
                return entryNames(path, flags).size();
            }

            /**
             * @brief
             * @param entryName
             * @return
             */
            inline bool hasEntry(const std::string& entryName) const
            {
                if (!isOpen()) throw ZipLogicError("Cannot call HasEntry on empty ZipArchive object!");

                return std::find_if(m_zipEntryData.begin(), m_zipEntryData.end(), [&](const ZipEntryWrapper& data) {
                           return strcmp(entryName.c_str(), data.entry().stats().m_filename) == 0;
                       }) != m_zipEntryData.end();    // NOLINT
            }

            /**
             * @brief Close the archive for reading and writing.
             * @note If the archive has been modified but not saved, all changes will be discarded.
             */
            inline void close()
            {
                if (isOpen()) {
                    mz_zip_reader_end(&m_archive);
                }
                m_zipEntryData.clear();
                m_archivePath.clear();
                m_isOpen = false;
            }

            /**
             * @brief Checks if the archive file is open for reading and writing.
             * @return true if it is open; otherwise false;
             */
            inline bool isOpen() const { return m_isOpen; }

            /**
             * @brief Save the archive with a new name. The original archive will remain unchanged.
             * @param filename The new filename/path.
             * @note If no filename is provided, the file will be saved with the existing name, overwriting any existing data.
             * @throws ZipException A ZipException object is thrown if calls to miniz function fails.
             */
            inline void save(fs::path filename = {})
            {
                if (!isOpen()) throw ZipLogicError("Function call: save(). Archive is invalid or not open!");

                if (filename.empty()) {
                    filename = m_archivePath;
                }

                // ===== Lambda function for generating a gandom filename
                auto generateRandomName = []() {
                    static const std::string letters = "abcdefghijklmnopqrstuvwxyz0123456789";

                    std::random_device                          rand_dev;
                    std::mt19937                                generator(rand_dev());
                    std::uniform_int_distribution<unsigned int> distr(0, letters.size() - 1);

                    std::string result = ".~$";
                    for (int i = 0; i < 12; ++i) {    // NOLINT
                        result += letters[distr(generator)];
                    }

                    return result + ".tmp";
                };

                // ===== Generate a random file name with the same path as the current file
                fs::path tempPath = filename.parent_path() /= generateRandomName();

                // ===== Prepare an temporary archive file with the random filename;
                mz_zip_archive tempArchive = mz_zip_archive();
                mz_zip_writer_init_file(&tempArchive, tempPath.string().c_str(), 0);

                // ===== Iterate through the ZipEntries and add entries to the temporary file
                for (auto& entry : m_zipEntryData) {
                    if (entry.entry().stats().m_is_directory) continue;
                    if (!entry.entry().isUpdated()) {
                        if (!mz_zip_writer_add_from_zip_reader(&tempArchive, &m_archive, entry.entry().stats().m_file_index)) {
                            throw ZipRuntimeError(mz_zip_get_error_string(m_archive.m_last_error));
                        }
                    }

                    else {
                        if (!mz_zip_writer_add_mem(&tempArchive,
                                                   entry.entry().stats().m_filename,    // NOLINT
                                                   entry.entry().rawData().data(),
                                                   entry.entry().rawData().size(),
                                                   MZ_DEFAULT_COMPRESSION))
                        {
                            throw ZipRuntimeError(mz_zip_get_error_string(m_archive.m_last_error));
                        }
                    }
                }

                // ===== Finalize and close the temporary archive
                mz_zip_writer_finalize_archive(&tempArchive);
                mz_zip_writer_end(&tempArchive);

                // ===== Validate the temporary file
                mz_zip_error errordata = {};
                if (!mz_zip_validate_file_archive(tempPath.string().c_str(), 0, &errordata)) {
                    throw ZipRuntimeError(mz_zip_get_error_string(errordata));
                }

                // ===== Close the current archive, delete the file with input filename (if it exists), rename the temporary and call Open.
                close();
                nowide::remove(filename.string().c_str());                      // NOLINT
                nowide::rename(tempPath.string().c_str(), filename.string().c_str());    // NOLINT
                open(filename);
            }

        private:
            /**
             * @brief Create a new mz_zip_archive_file_stat structure.
             * @details This function will create a new mz_zip_archive_file_stat structure, based on the input file name. The
             * structure values will mostly be dummy values, except for the file index, the time stamp, the file name
             * and the is_directory flag.
             * @param name The file name for the new ZipEntryInfo.
             * @return The newly created ZipEntryInfo structure is returned
             */
            inline mz_zip_archive_file_stat createInfo(const std::string& name)
            {
                mz_zip_archive_file_stat info;

                info.m_file_index       = ++m_currentIndex;
                info.m_central_dir_ofs  = 0;
                info.m_version_made_by  = 0;
                info.m_version_needed   = 0;
                info.m_bit_flag         = 0;
                info.m_method           = 0;
                info.m_time             = std::time(nullptr);
                info.m_crc32            = 0;
                info.m_comp_size        = 0;
                info.m_uncomp_size      = 0;
                info.m_internal_attr    = 0;
                info.m_external_attr    = 0;
                info.m_local_header_ofs = 0;
                info.m_comment_size     = 0;
                info.m_is_directory     = (name.back() == '/');
                info.m_is_encrypted     = false;
                info.m_is_supported     = true;

#if _MSC_VER    // On MSVC, use the safe version of strcpy
                strcpy_s(info.m_filename, sizeof info.m_filename, name.c_str());
                strcpy_s(info.m_comment, sizeof info.m_comment, "");
#else    // Otherwise, use the unsafe version as fallback :(
                strncpy(info.m_filename, name.c_str(), sizeof info.m_filename);    // NOLINT
                strncpy(info.m_comment, "", sizeof info.m_comment);                // NOLINT
#endif

                return info;
            }

            mz_zip_archive    m_archive      = mz_zip_archive(); /**< The struct used by miniz, to handle archive files. */
            std::vector<ZipEntryWrapper> m_zipEntryData = {};
            fs::path          m_archivePath  = {}; /**< The path of the archive file. */
            bool              m_isOpen { false };  /**< A flag indicating if the file is currently open for reading and writing. */
            uint32_t          m_currentIndex { 0 };
        };
    }    // namespace Impl

    /**
     * @brief The ZipArchive class represents the zip archive file as a whole. It consists of the individual zip entries, which
     * can be both files and folders. It is the main access point into a .zip archive on disk and can be
     * used to create new archives and to open and modify existing archives.
     * @details
     * #### Implementation and usage details
     * Using the ZipArchive class, it is possible to create new .zip archive files, as well as open and modify existing ones.
     *
     * A ZipArchive object holds a mz_zip_archive object (a miniz struct representing a .zip archive) as well as a std::vector
     * with ZipEntry objects representing each file (entry) in the archive. The actual entry data is lazy-instantiated, so that
     * the data is only loaded from the .zip archive when it is actually needed.
     *
     * The following example shows how a new .zip archive can be created and new entries added.
     * ```cpp
     * int main() {
     *
     *       // ===== Creating empty archive
     *       KZip::ZipArchive arch;
     *       arch.Create("./TestArchive.zip");
     *
     *       // ===== Adding 10 entries to the archive
     *       for (int i = 0; i <= 9; ++i)
     *           arch.AddEntry(to_string(i) + ".txt", "this is test " + to_string(i));
     *
     *       // ===== Delete the first entry
     *       arch.DeleteEntry("0.txt");
     *
     *       // ===== Save and close the archive
     *       arch.Save();
     *       arch.Close();
     *
     *       // ===== Reopen and check contents
     *       arch.Open("./TestArchive.zip");
     *       cout << "Number of entries in archive: " << arch.GetNumEntries() << endl;
     *       cout << "Content of \"9.txt\": " << endl << arch.GetEntry("9.txt").GetDataAsString();
     *
     *       return 0;
     *   }
     * ```
     *
     * For further information, please refer to the full API documentation below.
     *
     * Note that the actual files in the .zip archive can be retrieved via the ZipEntry interface, not the ZipArchive interface.
     */
    class ZipArchive
    {
    public:
        /**
         * @brief Constructor. Constructs a null-archive, which can be used for creating a new .zip file
         * or opening an existing one.
         * @warning Before using an archive object that has been default constructed, a call to either Open() or Create() must be
         * performed. Otherwise, the object will be in a null-state and calls to member functions will be undefined.
         */
        ZipArchive() = default;

        /**
         * @brief Constructor. Constructs an archive object, using the fileName input parameter. If the file already exists,
         * it will be opened. Otherwise, a new object will be created.
         * @details
         * #### Implementation details
         * The constructors tries to open an std::ifstream. If it is valid, it means that a file already exists
         * and will be opened. Otherwise, the file does not exist and will be created.
         * @param fileName The name of the file to open or create.
         */
        explicit ZipArchive(const fs::path& fileName)
        {
            // ===== If successful, continue to open the file.
            if (fs::exists(fileName)) open(fileName);

            // ===== If unsuccessful, create the archive file and continue.
            else
                create(fileName);
        }

        /**
         * @brief Copy Constructor (deleted).
         * @param other The object to copy.
         * @note The copy constructor has been deleted, because it is not obvious what should happen to the underlying .zip file
         * when copying the archive object. Instead, if sharing of the resource is required, a std::shared_ptr can be used.
         */
        ZipArchive(const ZipArchive& other) = delete;

        /**
         * @brief Move Constructor.
         * @param other The object to be moved.
         */
        ZipArchive(ZipArchive&& other) = default;

        /**
         * @brief Destructor.
         * @note The destructor will call the Close() member function. If the archive has been modified but not saved,
         * all changes will be discarded.
         */
        virtual ~ZipArchive() { close(); }

        /**
         * @brief Copy Assignment Operator (deleted)
         * @param other The object to be copied.
         * @return A reference to the copied-to object.
         * @note The copy assignment operator has been deleted, because it is not obvious what should happen to the underlying
         * .zip file when copying the archive object. Instead, if sharing of the resource is required,
         * a std::shared_ptr can be used.
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
         * @details
         * #### Implementation details
         * A new archive file is created and initialized and thereafter closed, creating an empty archive file.
         * After ensuring that the file is valid (i.e. not somehow corrupted), it is opened using the Open() member function.
         * @param fileName The filename for the new archive.
         */
        inline void create(const fs::path& fileName) { m_archive->create(fileName); }

        /**
         * @brief Open an existing archive file with the given filename.
         * @details
         * ##### Implementation details
         * The archive file is opened and meta data for all the entries in the archive is loaded into memory.
         * @param fileName The filename of the archive to open.
         * @note If more than one entry with the same name exists in the archive, only the newest one will be loaded.
         * When saving the archive, only the loaded entries will be kept; other entries with the same name will be deleted.
         */
        inline void open(const fs::path& fileName) { m_archive->open(fileName); }

        /**
         * @brief Close the archive for reading and writing.
         * @note If the archive has been modified but not saved, all changes will be discarded.
         */
        inline void close() { m_archive->close(); }

        /**
         * @brief Checks if the archive file is open for reading and writing.
         * @return true if it is open; otherwise false;
         */
        inline bool isOpen() const { return m_archive->isOpen(); }

        /**
         * @brief Get a list of the entries in the archive. Depending on the input parameters, the list will include
         * directories, files or both.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of std::strings with the entry names.
         */
        inline std::vector<std::string> entryNames(ZipFlags flags = (ZipFlags::Files)) const
        {
            return m_archive->entryNames(flags);
        }

        /**
         * @brief
         * @param path
         * @param flags
         * @return
         */
        inline std::vector<std::string> entryNames(const std::string& path, ZipFlags flags = (ZipFlags::Files)) const
        {
            return m_archive->entryNames(path, flags);
        }

        /**
         * @brief
         * @param flags
         * @return
         */
        inline uint16_t entryCount(ZipFlags flags = (ZipFlags::Files))
        {
            return m_archive->entryCount(flags);
        }

        /**
         * @brief
         * @param path
         * @param flags
         * @return
         */
        inline uint16_t entryCount(const std::string& path, ZipFlags flags = (ZipFlags::Files))
        {
            return m_archive->entryCount(path, flags);
        }

        /**
         * @brief Check if an entry with a given name exists in the archive.
         * @param entryName The name of the entry to check for.
         * @return true if it exists; otherwise false;
         */
        inline bool hasEntry(const std::string& entryName) const { return m_archive->hasEntry(entryName); }

        /**
         * @brief Save the archive with a new name. The original archive will remain unchanged.
         * @param filename The new filename.
         * @note If no filename is provided, the file will be saved with the existing name, overwriting any existing data.
         * @throws ZipException A ZipException object is thrown if calls to miniz function fails.
         */
        inline void save(const fs::path& filename = "") { m_archive->save(filename); }

        /**
         * @brief Deletes an entry from the archive.
         * @param name The name of the entry to delete.
         */
        inline void deleteEntry(const std::string& name) { m_archive->deleteEntry(name); }

        /**
         *
         * @param path
         * @return
         */
        inline ZipEntryProxy& entry(const std::string& path) { return m_archive->entry(path); }

        inline const ZipEntryProxy& entry(const std::string& path) const { return m_archive->entry(path); }

        //        inline void extractEntry(const std::string& name, const std::string& dest)
        //        inline void extractDir(const std::string& dir, const std::string& dest)
        //        inline void extractAll(const std::string& dest)

        /**
         * @brief Add a new entry to the archive.
         * @param name The name of the entry to add.
         * @param data The ZipEntryData to add. This is an alias of a std::vector<std::byte>, holding the binary data of the entry.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
        inline ZipEntryProxy& addEntry(const std::string& name)
        {
            //            return addEntryImpl(name, data);
            return m_archive->addEntry(name);
        }

        /**
         * @brief Add a new entry to the archive.
         * @param name The name of the entry to add.
         * @param data The std::string data to add.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
//        inline void addEntry(const std::string& name, const std::string& data) { m_archive->addEntry(name, { data.begin(), data.end() }); }

        /**
         * @brief Add a new entry to the archive, using another entry as input. This is mostly used for cloning existing entries
         * in the same archive, or for copying entries from one archive to another.
         * @param name The name of the entry to add.
         * @param entry The ZipEntry to add.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
//        inline void addEntry(const std::string& name, const ZipEntry& entry)
//        {
//            //            return addEntryImpl(name, entry.getData());
//        }

    private:
        std::unique_ptr<Impl::ZipArchive> m_archive = std::make_unique<Impl::ZipArchive>();
    };
}    // namespace KZip

#pragma warning(pop)
#endif    // KZIP_HPP
