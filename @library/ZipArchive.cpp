//
// Created by Troldal on 2019-03-10.
//

#include "ZipArchive.h"
#include "ZipException.h"
#include "miniz/miniz.h"

#include <algorithm>
#include <random>
#include <iostream>
#include <cstdio>
#include <cstddef>
#include <memory>
#include <fstream>

using namespace Zippy;

ZipArchive::ZipArchive(const std::string& fileName)
        : m_ArchivePath(fileName) {

    std::ifstream f(fileName.c_str());
    if (f.good()) {
        f.close();
        Open(fileName);
    }
    else {
        f.close();
        Create(fileName);
    }

}

ZipArchive::~ZipArchive() {

    Close();
}

void ZipArchive::Create(const std::string& fileName) {

    // ===== Prepare an archive file;
    mz_zip_archive archive = mz_zip_archive();
    mz_zip_writer_init_file(&archive, fileName.c_str(), 0);

    // ===== Finalize and close the temporary archive
    mz_zip_writer_finalize_archive(&archive);
    mz_zip_writer_end(&archive);

    // ===== Validate the temporary file
    mz_zip_error errordata;
    if (!mz_zip_validate_file_archive(fileName.c_str(), 0, &errordata))
        ThrowException(errordata, "Archive creation failed!");

    Open(fileName);

}

void ZipArchive::Open(const std::string& fileName) {

    // ===== Open the archive file for reading.
    if (m_IsOpen) mz_zip_reader_end(&m_Archive);
    m_ArchivePath = fileName;
    if (!mz_zip_reader_init_file(&m_Archive, m_ArchivePath.c_str(), 0))
        ThrowException(m_Archive.m_last_error, "Error opening archive file " + fileName + ".");
    m_IsOpen = true;

    // ===== Iterate through the archive and add the entries to the internal data structure
    for (unsigned int i = 0; i < mz_zip_reader_get_num_files(&m_Archive); i++) {
        ZipEntryInfo info;
        if (!mz_zip_reader_file_stat(&m_Archive, i, &info))
            ThrowException(m_Archive.m_last_error, "Failed getting entry info.");

        m_ZipEntries.emplace_back(info);
    }

    // ===== Remove entries with identical names. The newest entries will be retained.
    auto isEqual = [](const Impl::ZipEntry& a, const Impl::ZipEntry& b) { return a.Filename() == b.Filename(); };
    std::reverse(m_ZipEntries.begin(), m_ZipEntries.end());
    m_ZipEntries.erase(std::unique(m_ZipEntries.begin(), m_ZipEntries.end(), isEqual), m_ZipEntries.end());
    std::reverse(m_ZipEntries.begin(), m_ZipEntries.end());
}

void ZipArchive::Close() {

    if (m_IsOpen) mz_zip_reader_end(&m_Archive);
    m_ZipEntries.clear();
    m_ArchivePath = "";
}

bool ZipArchive::IsOpen() {

    return m_IsOpen;
}

std::vector<std::string> ZipArchive::GetEntryNames(bool includeDirs, bool includeFiles) {

    std::vector<std::string> result;
    for (auto& item : m_ZipEntries) {
        if (includeDirs && item.IsDirectory()) {
            result.emplace_back(item.Filename());
            continue;
        }

        if (includeFiles && !item.IsDirectory()) {
            result.emplace_back(item.Filename());
            continue;
        }
    }

    return result;
}

std::vector<std::string> ZipArchive::GetEntryNamesInDir(const std::string& dir, bool includeDirs, bool includeFiles) {

    auto result = GetEntryNames(includeDirs, includeFiles);

    if (dir.empty()) return result;

    auto theDir = dir;
    if (theDir.back() != '/') theDir += '/';

    result.erase(std::remove_if(result.begin(), result.end(), [&](const std::string& filename) {
        return filename.substr(0, theDir.size()) != theDir || filename == theDir;
    }), result.end());

    return result;
}

std::vector<ZipEntryMetaData> ZipArchive::GetMetaData(bool includeDirs, bool includeFiles) {

    std::vector<ZipEntryMetaData> result;
    for (auto& item : m_ZipEntries) {
        if (includeDirs && item.IsDirectory()) {
            result.emplace_back(ZipEntryMetaData(item.m_EntryInfo));
            continue;
        }

        if (includeFiles && !item.IsDirectory()) {
            result.emplace_back(ZipEntryMetaData(item.m_EntryInfo));
            continue;
        }
    }

    return result;
}

std::vector<ZipEntryMetaData> ZipArchive::GetMetaDataInDir(const std::string& dir, bool includeDirs, bool includeFiles) {

    std::vector<ZipEntryMetaData> result;
    for (auto& item : m_ZipEntries) {
        if (item.Filename().substr(0, dir.size()) != dir) continue;

        if (includeDirs && item.IsDirectory()) {
            result.emplace_back(ZipEntryMetaData(item.m_EntryInfo));
            continue;
        }

        if (includeFiles && !item.IsDirectory()) {
            result.emplace_back(ZipEntryMetaData(item.m_EntryInfo));
            continue;
        }
    }

    return result;
}

int ZipArchive::GetNumEntries(bool includeDirs, bool includeFiles) {

    return GetEntryNames(includeDirs, includeFiles).size();
}

int ZipArchive::GetNumEntriesInDir(const std::string& dir, bool includeDirs, bool includeFiles) {

    return GetEntryNamesInDir(dir, includeDirs, includeFiles).size();
}

bool ZipArchive::HasEntry(const std::string& entryName, bool includeDirs, bool includeFiles) {

    auto result = GetEntryNames(includeDirs, includeFiles);
    return std::find(result.begin(), result.end(), entryName) != result.end();
}

void ZipArchive::Save() {

    SaveAs(m_ArchivePath);
}

void ZipArchive::SaveAs(std::string filename) {

    // ===== Generate a random file name with the same path as the current file
    std::string tempPath = filename.substr(0, filename.rfind('/') + 1) + GenerateRandomName();

    // ===== Prepare an temporary archive file with the random filename;
    mz_zip_archive tempArchive = mz_zip_archive();
    mz_zip_writer_init_file(&tempArchive, tempPath.c_str(), 0);

    // ===== Iterate through the ZipEntries and add entries to the temporary file
    for (auto& file : m_ZipEntries) {
        if (!file.IsModified()) {
            if (!mz_zip_writer_add_from_zip_reader(&tempArchive, &m_Archive, file.Index()))
                ThrowException(m_Archive.m_last_error, "Failed copying archive entry.");
        }

        else {
            if (!mz_zip_writer_add_mem(&tempArchive,
                                       file.Filename().c_str(),
                                       file.m_EntryData.data(),
                                       file.m_EntryData.size(),
                                       MZ_DEFAULT_COMPRESSION))
                ThrowException(m_Archive.m_last_error, "Failed adding archive entry.");
        }

    }

    // ===== Finalize and close the temporary archive
    mz_zip_writer_finalize_archive(&tempArchive);
    mz_zip_writer_end(&tempArchive);

    // ===== Validate the temporary file
    mz_zip_error errordata;
    if (!mz_zip_validate_file_archive(tempPath.c_str(), 0, &errordata))
        ThrowException(errordata, "Invalid archive");

    // ===== Close the current archive, delete the file with input filename (if it exists), rename the temporary and call Open.
    Close();
    std::remove(filename.c_str());
    std::rename(tempPath.c_str(), filename.c_str());
    Open(filename);

}

void ZipArchive::DeleteEntry(const std::string& name) {

    m_ZipEntries.erase(std::remove_if(m_ZipEntries.begin(), m_ZipEntries.end(), [&](const Impl::ZipEntry& entry) {
        return name == entry.Filename();
    }), m_ZipEntries.end());
}

ZipEntry ZipArchive::GetEntry(const std::string& name) {

    auto result = std::find_if(m_ZipEntries.begin(), m_ZipEntries.end(), [&](const Impl::ZipEntry& entry) {
        return name == entry.Filename();
    });

    size_t datasize;

    std::unique_ptr<std::byte, decltype(&mz_free)> data(static_cast<std::byte*>(mz_zip_reader_extract_file_to_heap(&m_Archive,
                                                                                                                   name.c_str(),
                                                                                                                   &datasize,
                                                                                                                   0)), &mz_free);

    if (!data)
        ThrowException(m_Archive.m_last_error, "Error extracting archive entry.");

    result->SetData(std::vector<std::byte>(data.get(), data.get() + datasize));

    return ZipEntry(&*result);
}

// TODO: The three AddEntry functions are essentially identical. Can something be done?

ZipEntry ZipArchive::AddEntry(const std::string& name, const ZipEntryData& data) {

    auto result = std::find_if(m_ZipEntries.begin(), m_ZipEntries.end(), [&](const Impl::ZipEntry& entry) {
        return name == entry.Filename();
    });

    if (result != m_ZipEntries.end())
        return ZipEntry(&*result);

    return ZipEntry(&m_ZipEntries.emplace_back(Impl::ZipEntry(name, data)));
}

ZipEntry ZipArchive::AddEntry(const std::string& name, const std::string& data) {

    auto result = std::find_if(m_ZipEntries.begin(), m_ZipEntries.end(), [&](const Impl::ZipEntry& entry) {
        return name == entry.Filename();
    });

    if (result != m_ZipEntries.end())
        return ZipEntry(&*result);

    return ZipEntry(&m_ZipEntries.emplace_back(Impl::ZipEntry(name, data)));
}

ZipEntry ZipArchive::AddEntry(const std::string& name, const ZipEntry& entry) {

    // TODO: Ensure to check for self-asignment.
    auto result = std::find_if(m_ZipEntries.begin(), m_ZipEntries.end(), [&](const Impl::ZipEntry& entry) {
        return name == entry.Filename();
    });

    if (result != m_ZipEntries.end())
        return ZipEntry(&*result);

    return ZipEntry(&m_ZipEntries.emplace_back(Impl::ZipEntry(name, entry.GetData())));
}

void ZipArchive::ThrowException(mz_zip_error error, const std::string& errorString) {

    switch (error) {
        case MZ_ZIP_UNDEFINED_ERROR:
            throw ZipExceptionUndefined(errorString);

        case MZ_ZIP_TOO_MANY_FILES:
            throw ZipExceptionTooManyFiles(errorString);

        case MZ_ZIP_FILE_TOO_LARGE:
            throw ZipExceptionFileTooLarge(errorString);

        case MZ_ZIP_UNSUPPORTED_METHOD:
            throw ZipExceptionUnsupportedMethod(errorString);

        case MZ_ZIP_UNSUPPORTED_ENCRYPTION:
            throw ZipExceptionUnsupportedEncryption(errorString);

        case MZ_ZIP_UNSUPPORTED_FEATURE:
            throw ZipExceptionUnsupportedFeature(errorString);

        case MZ_ZIP_FAILED_FINDING_CENTRAL_DIR:
            throw ZipExceptionFailedFindingCentralDir(errorString);

        case MZ_ZIP_NOT_AN_ARCHIVE:
            throw ZipExceptionNotAnArchive(errorString);

        case MZ_ZIP_INVALID_HEADER_OR_CORRUPTED:
            throw ZipExceptionInvalidHeader(errorString);

        case MZ_ZIP_UNSUPPORTED_MULTIDISK:
            throw ZipExceptionMultidiskUnsupported(errorString);

        case MZ_ZIP_DECOMPRESSION_FAILED:
            throw ZipExceptionDecompressionFailed(errorString);

        case MZ_ZIP_COMPRESSION_FAILED:
            throw ZipExceptionCompressionFailed(errorString);

        case MZ_ZIP_UNEXPECTED_DECOMPRESSED_SIZE:
            throw ZipExceptionUnexpectedDecompSize(errorString);

        case MZ_ZIP_CRC_CHECK_FAILED:
            throw ZipExceptionCrcCheckFailed(errorString);

        case MZ_ZIP_UNSUPPORTED_CDIR_SIZE:
            throw ZipExceptionUnsupportedCDirSize(errorString);

        case MZ_ZIP_ALLOC_FAILED:
            throw ZipExceptionAllocFailed(errorString);

        case MZ_ZIP_FILE_OPEN_FAILED:
            throw ZipExceptionFileOpenFailed(errorString);

        case MZ_ZIP_FILE_CREATE_FAILED:
            throw ZipExceptionFileCreateFailed(errorString);

        case MZ_ZIP_FILE_WRITE_FAILED:
            throw ZipExceptionFileWriteFailed(errorString);

        case MZ_ZIP_FILE_READ_FAILED:
            throw ZipExceptionFileReadFailed(errorString);

        case MZ_ZIP_FILE_CLOSE_FAILED:
            throw ZipExceptionFileCloseFailed(errorString);

        case MZ_ZIP_FILE_SEEK_FAILED:
            throw ZipExceptionFileSeekFailed(errorString);

        case MZ_ZIP_FILE_STAT_FAILED:
            throw ZipExceptionFileStatFailed(errorString);

        case MZ_ZIP_INVALID_PARAMETER:
            throw ZipExceptionInvalidParameter(errorString);

        case MZ_ZIP_INVALID_FILENAME:
            throw ZipExceptionInvalidFilename(errorString);

        case MZ_ZIP_BUF_TOO_SMALL:
            throw ZipExceptionBufferTooSmall(errorString);

        case MZ_ZIP_INTERNAL_ERROR:
            throw ZipExceptionInternalError(errorString);

        case MZ_ZIP_FILE_NOT_FOUND:
            throw ZipExceptionFileNotFound(errorString);

        case MZ_ZIP_ARCHIVE_TOO_LARGE:
            throw ZipExceptionArchiveTooLarge(errorString);

        case MZ_ZIP_VALIDATION_FAILED:
            throw ZipExceptionValidationFailed(errorString);

        case MZ_ZIP_WRITE_CALLBACK_FAILED:
            throw ZipExceptionWriteCallbackFailed(errorString);

        default:
            throw ZipException(errorString);

    }
}

std::string ZipArchive::GenerateRandomName() {

    std::string letters = "abcdefghijklmnopqrstuvwxyz0123456789";

    auto                               range_from = 0;
    auto                               range_to   = letters.size() - 1;
    std::random_device                 rand_dev;
    std::mt19937                       generator(rand_dev());
    std::uniform_int_distribution<int> distr(range_from, range_to);

    std::string result;
    for (int    i                                 = 0; i < 20; ++i) {
        result += letters[distr(generator)];
    }

    return result + ".tmp";
}
