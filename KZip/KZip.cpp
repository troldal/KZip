//
// Created by Kenneth Balslev on 18/08/2022.
//


#include "KZip.hpp"

namespace KZip {

        ZipEntry::ZipEntry(const std::string& filename) {
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

        ZipEntry::ZipEntry(const ZipEntry& other) = default;

        ZipEntry::ZipEntry(ZipEntry&& other) noexcept = default;

        ZipEntry& ZipEntry::operator=(const ZipEntry& other) = default;

        ZipEntry& ZipEntry::operator=(ZipEntry&& other) noexcept = default;

        ZipEntry::~ZipEntry() = default;

        std::string_view ZipEntry::name() const {
            return {m_info.m_filename};
        }

        void ZipEntry::setName(const std::string& entryname) {
            if (entryname.empty()) throw ZipLogicError("Entry name must not be empty");

#if _MSC_VER    // On MSVC, use the safe version of strcpy
            strcpy_s(m_info.m_filename, sizeof m_info.m_filename, entryname.c_str());
            strcpy_s(m_info.m_comment, sizeof m_info.m_comment, "");
#else    // Otherwise, use the unsafe version as fallback :(
            strncpy(m_info.m_filename, entryname.c_str(), sizeof m_info.m_filename);    // NOLINT
            strncpy(m_info.m_comment, "", sizeof m_info.m_comment);                // NOLINT
#endif
        }

        ZipEntryMetaData ZipEntry::metadata() const { return ZipEntryMetaData(m_info); }

        ZipEntry::ZipEntry(std::vector<unsigned char> data, mz_zip_archive_file_stat info) : m_data(std::move(data)), m_info(info) {}
} // namespace KZip

namespace KZip {

    ZipEntryProxy::~ZipEntryProxy() = default;

    ZipEntryProxy& ZipEntryProxy::operator=(const ZipEntryProxy& other) {
        if (&other != this) {
            *this = other.getData<std::vector<unsigned char>>();
        }

        return *this;
    };

    ZipEntryProxy::operator ZipEntry() { // NOLINT
        return {getData<std::vector<unsigned char>>(), m_info};
    }

    void ZipEntryProxy::clear()
    {
        // TODO(troldal): To be implemented
    }

    std::string_view ZipEntryProxy::name() const {
        return {m_info.m_filename};
    }

    void ZipEntryProxy::setName(const std::string& entryname) {
        if (entryname.empty()) throw ZipLogicError("Entry name must not be empty");
        if (name() == entryname) return;
        if (m_ziparchive->hasEntry(entryname)) m_ziparchive->deleteEntry(entryname);

#if _MSC_VER    // On MSVC, use the safe version of strcpy
        strcpy_s(m_info.m_filename, sizeof m_info.m_filename, entryname.c_str());
        strcpy_s(m_info.m_comment, sizeof m_info.m_comment, "");
#else    // Otherwise, use the unsafe version as fallback :(
        strncpy(m_info.m_filename, entryname.c_str(), sizeof m_info.m_filename);    // NOLINT
        strncpy(m_info.m_comment, "", sizeof m_info.m_comment);                // NOLINT
#endif

    }

    ZipEntryMetaData ZipEntryProxy::metadata() const { return ZipEntryMetaData(m_info); }

    ZipEntryProxy::ZipEntryProxy(KZip::Impl::ZipArchive* archive, mz_zip_archive_file_stat info) : m_ziparchive(archive),
                                                                                                   m_archive(&archive->m_archive),
                                                                                                   m_info(info) {}

    ZipEntryProxy::ZipEntryProxy(const ZipEntryProxy& other) = default;

    ZipEntryProxy::ZipEntryProxy(ZipEntryProxy&& other) noexcept = default;

    ZipEntryProxy& ZipEntryProxy::operator=(ZipEntryProxy&& other) noexcept = default;

    const mz_zip_archive_file_stat& ZipEntryProxy::stats() const {
        return m_info;
    }

    bool ZipEntryProxy::isUpdated() const {
        return m_data.has_value();
    }

    uint64_t ZipEntryProxy::size() const {
        if (isUpdated())
            return m_data.has_value() ? m_data.value().size() : 0;

        return m_info.m_uncomp_size;
    }

    const std::vector<unsigned char>& ZipEntryProxy::rawData() const {
        return m_data.value();
    }

} // namespace KZip

namespace KZip::Impl {

    ZipEntryWrapper::ZipEntryWrapper(const ZipEntryProxy& entry) : m_entry(entry) {}

    const ZipEntryProxy& ZipEntryWrapper::entry() const {
        return m_entry;
    }

    ZipEntryProxy& ZipEntryWrapper::entry() {
        return m_entry;
    }


    ZipArchive::ZipArchive() = default;

    ZipArchive::ZipArchive(ZipArchive&& other) noexcept = default;

    ZipArchive::~ZipArchive() { close(); }

    ZipArchive& ZipArchive::operator=(ZipArchive&& other) = default;

    void ZipArchive::create(const fs::path& fileName)
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

    void ZipArchive::open(const fs::path& fileName)
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
            m_zipEntryData.emplace_back(ZipEntryProxy(this, info));
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

    ZipEntryProxy& ZipArchive::addEntry(const std::string& path)
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
                m_zipEntryData.emplace_back(ZipEntryProxy(this, createInfo(folderName)));
            }
        }

        // ===== Create a new entry and return the reference
        if (result != m_zipEntryData.end()) {
            *result = ZipEntryWrapper(ZipEntryProxy(this, createInfo(path)));
            return result->entry();
        }
        return m_zipEntryData.emplace_back(ZipEntryProxy(this, createInfo(path))).entry();
    }

    void ZipArchive::deleteEntry(const std::string& name)
    {
        if (!isOpen()) throw ZipLogicError("Function call: deleteEntry(). Archive is invalid or not open!");

        // ===== When saving, only the entries present in the vector will be saved or copied from the original file.
        m_zipEntryData.erase(std::remove_if(m_zipEntryData.begin(),
                                            m_zipEntryData.end(),
                                            [&](const ZipEntryWrapper& entry) { return strcmp(name.c_str(), entry.entry().stats().m_filename) == 0; }),
                             m_zipEntryData.end());
    }

    ZipEntryProxy& ZipArchive::entry(const std::string& path)
    {
        if (!isOpen()) throw ZipLogicError("Cannot get entry from empty ZipArchive object!");

        // ===== Look up file_stat object.
        auto stats = std::find_if(m_zipEntryData.begin(), m_zipEntryData.end(), [&](const ZipEntryWrapper& data) {
            return strcmp(path.c_str(), data.entry().stats().m_filename) == 0;
        });

        if (stats == m_zipEntryData.end()) throw ZipLogicError("KZip Error: Entry '" + path + "' does not exist");

        return stats->entry();
    }

    const ZipEntryProxy& ZipArchive::entry(const std::string& path) const
    {
        if (!isOpen()) throw ZipLogicError("Cannot get entry from empty ZipArchive object!");

        // ===== Look up file_stat object.
        auto stats = std::find_if(m_zipEntryData.begin(), m_zipEntryData.end(), [&](const ZipEntryWrapper& data) {
            return strcmp(path.c_str(), data.entry().stats().m_filename) == 0;
        });

        if (stats == m_zipEntryData.end()) throw ZipLogicError("KZip Error: Entry '" + path + "' does not exist");

        return stats->entry();
    }

    std::vector<std::string> ZipArchive::entryNames(ZipFlags flags) const
    {
        return entryNames("", flags);
    }

    std::vector<std::string> ZipArchive::entryNames(const std::string& path, ZipFlags flags) const
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

    uint16_t ZipArchive::entryCount(ZipFlags flags) const {
        return entryCount("", flags);
    }

    uint16_t ZipArchive::entryCount(const std::string& path, ZipFlags flags) const {
        return entryNames(path, flags).size();
    }

    bool ZipArchive::hasEntry(const std::string& entryName) const
    {
        if (!isOpen()) throw ZipLogicError("Cannot call HasEntry on empty ZipArchive object!");

        return std::find_if(m_zipEntryData.begin(), m_zipEntryData.end(), [&](const ZipEntryWrapper& data) {
                   return strcmp(entryName.c_str(), data.entry().stats().m_filename) == 0;
               }) != m_zipEntryData.end();    // NOLINT
    }

    void ZipArchive::close()
    {
        if (isOpen()) {
            mz_zip_reader_end(&m_archive);
        }
        m_zipEntryData.clear();
        m_archivePath.clear();
        m_isOpen = false;
    }

    bool ZipArchive::isOpen() const { return m_isOpen; }

    void ZipArchive::save(fs::path filename)
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

    mz_zip_archive_file_stat ZipArchive::createInfo(const std::string& name)
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

} // namespace KZip::Impl

namespace KZip {

    ZipArchive::ZipArchive() = default;

    ZipArchive::ZipArchive(const fs::path& fileName)
    {
        // ===== If successful, continue to open the file.
        if (fs::exists(fileName)) open(fileName);

        // ===== If unsuccessful, create the archive file and continue.
        else
            create(fileName);
    }

    ZipArchive::ZipArchive(ZipArchive&& other) noexcept = default;

    ZipArchive::~ZipArchive() { close(); }

    ZipArchive& ZipArchive::operator=(ZipArchive&& other) noexcept = default;

    void ZipArchive::create(const fs::path& fileName) { m_archive->create(fileName); }

    void ZipArchive::open(const fs::path& fileName) { m_archive->open(fileName); }

    void ZipArchive::close() { m_archive->close(); }

    bool ZipArchive::isOpen() const { return m_archive->isOpen(); }

    std::vector<std::string> ZipArchive::entryNames(ZipFlags flags) const
    {
        return m_archive->entryNames(flags);
    }

    std::vector<std::string> ZipArchive::entryNames(const std::string& path, ZipFlags flags) const
    {
        return m_archive->entryNames(path, flags);
    }

    uint16_t ZipArchive::entryCount(ZipFlags flags)
    {
        return m_archive->entryCount(flags);
    }

    uint16_t ZipArchive::entryCount(const std::string& path, ZipFlags flags)
    {
        return m_archive->entryCount(path, flags);
    }

    bool ZipArchive::hasEntry(const std::string& entryName) const { return m_archive->hasEntry(entryName); }

    void ZipArchive::save(const fs::path& filename) {
        m_archive->save(filename); }

    void ZipArchive::deleteEntry(const std::string& name) { m_archive->deleteEntry(name); }

    ZipEntryProxy& ZipArchive::entry(const std::string& path) { return m_archive->entry(path); }

    const ZipEntryProxy& ZipArchive::entry(const std::string& path) const { return m_archive->entry(path); }

    ZipEntryProxy& ZipArchive::addEntry(const std::string& name)
    {
        //            return addEntryImpl(name, data);
        return m_archive->addEntry(name);
    }



} // namespace KZip