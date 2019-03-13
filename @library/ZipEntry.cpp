//
// Created by Troldal on 2019-03-11.
//

#include "ZipEntry.h"
#include <iostream>

uint32_t SimpleZip::ZipEntry::s_LatestIndex = 0;

SimpleZip::ZipEntry::ZipEntry(const SimpleZip::ZipEntryInfo& info)
        : m_EntryInfo(info) {

    if (info.m_file_index > s_LatestIndex) s_LatestIndex = info.m_file_index;
}

SimpleZip::ZipEntry::ZipEntry(const std::string& name, const SimpleZip::ZipEntryData& data) {

    m_EntryInfo  = CreateInfo(name);
    m_EntryData  = data;
    m_IsModified = true;

}

SimpleZip::ZipEntry::ZipEntry(const std::string& name, const std::string& data) {

    m_EntryInfo = CreateInfo(name);
    m_EntryData.reserve(data.size());

    for (auto& ch : data)
        m_EntryData.emplace_back(static_cast<std::byte>(ch));

    m_IsModified = true;
}

SimpleZip::ZipEntryData SimpleZip::ZipEntry::GetData() const {

    return m_EntryData;
}

std::string SimpleZip::ZipEntry::GetDataAsString() const {

    std::string result;
    for (auto& ch : m_EntryData)
        result += static_cast<char>(ch);

    return result;
}

void SimpleZip::ZipEntry::SetData(const std::string& data) {

    ZipEntryData result;

    for (auto& ch : data)
        result.push_back(static_cast<std::byte>(ch));

    m_EntryData  = result;
    m_IsModified = true;
}

uint32_t SimpleZip::ZipEntry::Index() const {

    return m_EntryInfo.m_file_index;
}

uint64_t SimpleZip::ZipEntry::CompressedSize() const {

    return m_EntryInfo.m_comp_size;
}

uint64_t SimpleZip::ZipEntry::UncompressedSize() const {

    return m_EntryInfo.m_uncomp_size;
}

bool SimpleZip::ZipEntry::IsDirectory() const {

    return m_EntryInfo.m_is_directory;
}

bool SimpleZip::ZipEntry::IsEncrypted() const {

    return m_EntryInfo.m_is_encrypted;
}

bool SimpleZip::ZipEntry::IsSupported() const {

    return m_EntryInfo.m_is_supported;
}

std::string SimpleZip::ZipEntry::Filename() const {

    return m_EntryInfo.m_filename;
}

std::string SimpleZip::ZipEntry::Comment() const {

    return m_EntryInfo.m_comment;
}

const time_t& SimpleZip::ZipEntry::Time() const {

    return m_EntryInfo.m_time;
}

bool SimpleZip::ZipEntry::IsModified() const {

    return m_IsModified;
}

uint32_t SimpleZip::ZipEntry::GetNewIndex() {

    return ++s_LatestIndex;
}

SimpleZip::ZipEntryInfo SimpleZip::ZipEntry::CreateInfo(const std::string& name) {

    ZipEntryInfo info;

    info.m_file_index       = GetNewIndex();
    info.m_central_dir_ofs  = 0;
    info.m_version_made_by  = 0;
    info.m_version_needed   = 0;
    info.m_bit_flag         = 0;
    info.m_method           = 0;
    info.m_time             = time(nullptr);
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

    strcpy(info.m_filename, name.c_str());
    strcpy(info.m_comment, "");

    return info;
}
