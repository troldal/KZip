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
#include <type_traits>
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
    class ZipArchive;
    class ZipEntry;
    class ZipEntryProxy;

    namespace Impl
    {
        class ZipEntryWrapper;
        class ZipArchive;
    }    // namespace Impl



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
//        std::string_view comment() const { return m_stats.m_comment; }
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
        explicit ZipEntry(const std::string& filename);

        /**
         * @brief
         * @param other
         */
        ZipEntry(const ZipEntry& other);// = default;

        /**
         * @brief
         * @param other
         */
        ZipEntry(ZipEntry&& other) noexcept;// = default;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntry& operator=(const ZipEntry& other);// = default;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntry& operator=(ZipEntry&& other) noexcept;// = default;

        /**
         * @brief
         */
        ~ZipEntry();// = default;

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

            m_info.m_uncomp_size = m_data.size();
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
        template<typename T, typename std::enable_if<std::is_convertible_v<unsigned char, typename std::decay_t<T>::value_type> &&
                                                     !std::is_same_v<std::string_view, std::decay_t<T> > &&
                                                     !std::is_same_v<std::allocator<typename std::decay_t<T>::value_type>, std::decay_t<T> > &&
                                                     !std::is_same_v<std::initializer_list<typename std::decay_t<T>::value_type>, std::decay_t<T> > >::type* = nullptr>
        T data() const
        {
            using ReturnType = std::decay_t<T>;

            if constexpr (std::is_same_v<ReturnType, std::vector<unsigned char> >)
                return m_data;
            return ReturnType{m_data.begin(), m_data.end()};
        }

        /**
         * @brief Implicit type conversion operator.
         * @details This templated type conversion operator allows extraction of the zip data to any container
         * that holds a type convertible from unsigned char, and that can be constructed from begin and end
         * iterators from a source. This function simply calls the data<T>() function.
         * @note The SFINAE checks using std::allocator and std::initializer_list is required in order for conversion to std::string to work.
         * @tparam T The type to convert to (e.g. std::string or std::vector<unsigned char>
         * @return An object of type T, holding the zip data.
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<unsigned char, typename std::decay_t<T>::value_type> &&
                                                     !std::is_same_v<std::string_view, std::decay_t<T> > &&
                                                     !std::is_same_v<std::allocator<typename std::decay_t<T>::value_type>, std::decay_t<T> > &&
                                                     !std::is_same_v<std::initializer_list<typename std::decay_t<T>::value_type>, std::decay_t<T> > >::type* = nullptr>
        operator T() const // NOLINT
        {
            using ReturnType = std::decay_t<T>;
            return data<ReturnType>();
        }

        /**
         * @brief
         * @return
         */
        std::string_view name() const;

        /**
         * @brief
         * @param filename
         */
        void setName(const std::string& entryname);

        /**
         * @brief
         * @return
         */
        ZipEntryMetaData metadata() const;// { return ZipEntryMetaData(m_info); }

        /**
         * @brief
         * @tparam T
         * @param other
         * @return
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<unsigned char, typename std::decay_t<T>::value_type> ||
                                                     std::is_same_v<std::decay_t<T>, char*> >::type* = nullptr>
        bool operator==(const T& other) const // NOLINT
        {
            auto data_ = data<T>();
            auto result = std::mismatch(other.begin(), other.end(), data_.begin());
            return (result.first == other.end() || result.second == data_.end());
        }

    private:

        /**
         * @brief
         * @param archive
         * @param info
         */
        ZipEntry(std::vector<unsigned char> data, mz_zip_archive_file_stat info);// : m_data(std::move(data)), m_info(info) {}

        //---------- Private Member Variables ---------- //
        mz_zip_archive_file_stat m_info    = mz_zip_archive_file_stat(); /**< File stats for the entry. */
        std::vector<unsigned char> m_data = {};
    };

    class ZipEntryProxy {
        friend class Impl::ZipArchive;
        friend class Impl::ZipEntryWrapper;
    public:

        /**
         * @brief
         */
        ~ZipEntryProxy();// = default;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntryProxy& operator=(const ZipEntryProxy& other);

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
         * iterators from a source. This function simply calls the data<T>() function.
         * @tparam T The type to convert to (e.g. std::string or std::vector<unsigned char>
         * @return An object of type T, holding the zip data.
         */
        template<typename T, typename std::enable_if<std::is_convertible_v<unsigned char, typename T::value_type>>::type* = nullptr>
        operator T() const // NOLINT
        {
            return getData<T>();
        }

        /**
         * @brief
         * @return
         */
        operator ZipEntry(); // NOLINT

        /**
         * @brief
         */
        void clear();

        /**
         * @brief
         * @return
         */
        std::string_view name() const;

        /**
         * @brief
         * @param entryname
         */
        void setName(const std::string& entryname);

        /**
         * @brief
         * @return
         */
        ZipEntryMetaData metadata() const;

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
        ZipEntryProxy(KZip::Impl::ZipArchive* archive, mz_zip_archive_file_stat info);

        /**
         * @brief
         * @param other
         */
        ZipEntryProxy(const ZipEntryProxy& other);

        /**
         * @brief
         * @param other
         */
        ZipEntryProxy(ZipEntryProxy&& other) noexcept;

        /**
         * @brief
         * @param other
         * @return
         */
        ZipEntryProxy& operator=(ZipEntryProxy&& other) noexcept;

        /**
         *
         * @return
         */
        const mz_zip_archive_file_stat& stats() const;

        /**
         *
         * @return
         */
        bool isUpdated() const;

        /**
         * @brief
         * @return
         */
        uint64_t size() const ;

        /**
         * @brief
         * @return
         */
        const std::vector<unsigned char>& rawData() const ;


        //---------- Private Member Variables ---------- //
        KZip::Impl::ZipArchive*  m_ziparchive = nullptr;
        mz_zip_archive*          m_archive = nullptr;
        mz_zip_archive_file_stat m_info    = mz_zip_archive_file_stat(); /**< File stats for the entry. */
        std::optional<std::vector<unsigned char> > m_data = {};
    }; // class ZipEntryProxy

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
            explicit ZipEntryWrapper(const ZipEntryProxy& entry);

            /**
             * @brief
             * @return
             */
            const ZipEntryProxy& entry() const;

            /**
             * @brief
             * @return
             */
            ZipEntryProxy& entry();

        private:
            ZipEntryProxy m_entry;
        };

        /**
         * @brief
         */
        class ZipArchive
        {
            friend class KZip::ZipArchive;
            friend class KZip::ZipEntryProxy;

        public:
            /**
             * @brief
             */
            ZipArchive();// = default;

            /**
             * @brief
             * @param other
             */
            ZipArchive(const ZipArchive& other) = delete;

            /**
             * @brief
             * @param other
             */
            ZipArchive(ZipArchive&& other) noexcept;// = default;

            /**
             * @brief
             */
            ~ZipArchive();// { close(); }

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
            ZipArchive& operator=(ZipArchive&& other);// = default;

            /**
             * @brief Creates a new (empty) archive file with the given filename.
             * @details A new archive file is created and initialized and thereafter closed, creating an empty archive file.
             * After ensuring that the file is valid (i.e. not somehow corrupted), it is opened using the open() member function.
             * @param fileName The filename/path for the new archive.
             */
            void create(const fs::path& fileName);

            /**
             * @brief Open an existing archive file with the given filename.
             * @details The archive file is opened and meta data for all the entries in the archive is loaded into memory.
             * @param fileName The filename/path of the archive to open.
             * @note If more than one entry with the same name exists in the archive, only the newest one will be loaded.
             * When saving the archive, only the loaded entries will be kept; other entries with the same name will be deleted.
             */
            void open(const fs::path& fileName);

            /**
             * @brief Add an entry to the archive.
             * @details If an entry with the given name/path exists, it will be overwritten. Otherwise, a new entry will be created.
             * @note Any folders in the path that does not exist, will be created as separate entries.
             * @param path
             * @param data
             */
            ZipEntryProxy& addEntry(const std::string& path);

            /**
             * @brief
             * @param name
             */
            void deleteEntry(const std::string& name);

            /**
             * @brief
             * @param path
             * @return
             */
            ZipEntryProxy& entry(const std::string& path);

            /**
             * @brief
             * @param path
             * @return
             */
            const ZipEntryProxy& entry(const std::string& path) const;

            /**
             * @brief Get a list of the entries in the archive. Depending on the input parameters, the list will include
             * directories, files or both.
             * @param flags A ZipFlags enum indicating whether files and/or directories should be included. By default, only files are
             * included.
             * @return A std::vector of std::strings with the entry names.
             */
            std::vector<std::string> entryNames(ZipFlags flags) const;

            /**
             * @brief
             * @param path
             * @param flags
             * @return
             */
            std::vector<std::string> entryNames(const std::string& path, ZipFlags flags) const;

            /**
             * @brief
             * @param flags
             * @return
             */
            uint16_t entryCount(ZipFlags flags = (ZipFlags::Files)) const;

            /**
             * @brief
             * @param path
             * @param flags
             * @return
             */
            uint16_t entryCount(const std::string& path, ZipFlags flags = (ZipFlags::Files)) const;

            /**
             * @brief
             * @param entryName
             * @return
             */
            bool hasEntry(const std::string& entryName) const;

            /**
             * @brief Close the archive for reading and writing.
             * @note If the archive has been modified but not saved, all changes will be discarded.
             */
            void close();

            /**
             * @brief Checks if the archive file is open for reading and writing.
             * @return true if it is open; otherwise false;
             */
            bool isOpen() const;

            /**
             * @brief Save the archive with a new name. The original archive will remain unchanged.
             * @param filename The new filename/path.
             * @note If no filename is provided, the file will be saved with the existing name, overwriting any existing data.
             * @throws ZipException A ZipException object is thrown if calls to miniz function fails.
             */
            void save(fs::path filename = {});

        private:
            /**
             * @brief Create a new mz_zip_archive_file_stat structure.
             * @details This function will create a new mz_zip_archive_file_stat structure, based on the input file name. The
             * structure values will mostly be dummy values, except for the file index, the time stamp, the file name
             * and the is_directory flag.
             * @param name The file name for the new ZipEntryInfo.
             * @return The newly created ZipEntryInfo structure is returned
             */
            mz_zip_archive_file_stat createInfo(const std::string& name);

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
        ZipArchive();

        /**
         * @brief Constructor. Constructs an archive object, using the fileName input parameter. If the file already exists,
         * it will be opened. Otherwise, a new object will be created.
         * @details
         * #### Implementation details
         * The constructors tries to open an std::ifstream. If it is valid, it means that a file already exists
         * and will be opened. Otherwise, the file does not exist and will be created.
         * @param fileName The name of the file to open or create.
         */
        explicit ZipArchive(const fs::path& fileName);

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
        ZipArchive(ZipArchive&& other) noexcept;

        /**
         * @brief Destructor.
         * @note The destructor will call the Close() member function. If the archive has been modified but not saved,
         * all changes will be discarded.
         */
        ~ZipArchive();

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
        ZipArchive& operator=(ZipArchive&& other) noexcept;

        /**
         * @brief Creates a new (empty) archive file with the given filename.
         * @details
         * #### Implementation details
         * A new archive file is created and initialized and thereafter closed, creating an empty archive file.
         * After ensuring that the file is valid (i.e. not somehow corrupted), it is opened using the Open() member function.
         * @param fileName The filename for the new archive.
         */
        void create(const fs::path& fileName);

        /**
         * @brief Open an existing archive file with the given filename.
         * @details
         * ##### Implementation details
         * The archive file is opened and meta data for all the entries in the archive is loaded into memory.
         * @param fileName The filename of the archive to open.
         * @note If more than one entry with the same name exists in the archive, only the newest one will be loaded.
         * When saving the archive, only the loaded entries will be kept; other entries with the same name will be deleted.
         */
        void open(const fs::path& fileName);

        /**
         * @brief Close the archive for reading and writing.
         * @note If the archive has been modified but not saved, all changes will be discarded.
         */
        void close();

        /**
         * @brief Checks if the archive file is open for reading and writing.
         * @return true if it is open; otherwise false;
         */
        bool isOpen() const;

        /**
         * @brief Get a list of the entries in the archive. Depending on the input parameters, the list will include
         * directories, files or both.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of std::strings with the entry names.
         */
        std::vector<std::string> entryNames(ZipFlags flags = (ZipFlags::Files)) const;

        /**
         * @brief
         * @param path
         * @param flags
         * @return
         */
        std::vector<std::string> entryNames(const std::string& path, ZipFlags flags = (ZipFlags::Files)) const;

        /**
         * @brief
         * @param flags
         * @return
         */
        uint16_t entryCount(ZipFlags flags = (ZipFlags::Files));
//        {
//            return m_archive->entryCount(flags);
//        }

        /**
         * @brief
         * @param path
         * @param flags
         * @return
         */
        uint16_t entryCount(const std::string& path, ZipFlags flags = (ZipFlags::Files));

        /**
         * @brief Check if an entry with a given name exists in the archive.
         * @param entryName The name of the entry to check for.
         * @return true if it exists; otherwise false;
         */
        bool hasEntry(const std::string& entryName) const;

        /**
         * @brief Save the archive with a new name. The original archive will remain unchanged.
         * @param filename The new filename.
         * @note If no filename is provided, the file will be saved with the existing name, overwriting any existing data.
         * @throws ZipException A ZipException object is thrown if calls to miniz function fails.
         */
        void save(const fs::path& filename = {});

        /**
         * @brief Deletes an entry from the archive.
         * @param name The name of the entry to delete.
         */
        void deleteEntry(const std::string& name);

        /**
         *
         * @param path
         * @return
         */
        ZipEntryProxy& entry(const std::string& path);

        const ZipEntryProxy& entry(const std::string& path) const;

        /**
         * @brief Add a new entry to the archive.
         * @param name The name of the entry to add.
         * @param data The ZipEntryData to add. This is an alias of a std::vector<std::byte>, holding the binary data of the entry.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
        ZipEntryProxy& addEntry(const std::string& name);

    private:
        std::unique_ptr<Impl::ZipArchive> m_archive = std::make_unique<Impl::ZipArchive>();
    };
}    // namespace KZip

#pragma warning(pop)
#endif    // KZIP_HPP
