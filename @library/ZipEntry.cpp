//
// Created by Troldal on 2019-03-11.
//

#include "miniz/miniz.h"
#include "ZipEntry.h"
#include <iostream>

using namespace Zippy;

uint32_t Impl::ZipEntry::s_LatestIndex = 0;

ZipEntryMetaData::ZipEntryMetaData(const ZipEntryInfo& info)
        : Index(info.m_file_index),
          CompressedSize(info.m_comp_size),
          UncompressedSize(info.m_uncomp_size),
          IsDirectory(info.m_is_directory),
          IsEncrypted(info.m_is_encrypted),
          IsSupported(info.m_is_supported),
          Filename(info.m_filename),
          Comment(info.m_comment),
          Time(info.m_time) {

}

Impl::ZipEntry::ZipEntry(const Zippy::ZipEntryInfo& info)
        : m_EntryInfo(info) {

    if (info.m_file_index > s_LatestIndex) s_LatestIndex = info.m_file_index;
}

Impl::ZipEntry::ZipEntry(const std::string& name, const Zippy::ZipEntryData& data) {

    m_EntryInfo  = CreateInfo(name);
    m_EntryData  = data;
    m_IsModified = true;

}

Impl::ZipEntry::ZipEntry(const std::string& name, const std::string& data) {

    m_EntryInfo = CreateInfo(name);
    m_EntryData.reserve(data.size());

    for (auto& ch : data)
        m_EntryData.emplace_back(static_cast<std::byte>(ch));

    m_IsModified = true;
}

ZipEntryData Impl::ZipEntry::GetData() const {

    return m_EntryData;
}

std::string Impl::ZipEntry::GetDataAsString() const {

    std::string result;
    for (auto& ch : m_EntryData)
        result += static_cast<char>(ch);

    return result;
}

void Impl::ZipEntry::SetData(const std::string& data) {

    ZipEntryData result;

    for (auto& ch : data)
        result.push_back(static_cast<std::byte>(ch));

    m_EntryData  = result;
    m_IsModified = true;
}

void Impl::ZipEntry::SetData(const ZipEntryData& data) {

    m_EntryData  = data;
    m_IsModified = true;
}

// TODO: To be implemented
std::string Impl::ZipEntry::GetName() const {

    return std::string();
}

// TODO: To be implemented
void Impl::ZipEntry::SetName(const std::string& name) {

}

uint32_t Impl::ZipEntry::Index() const {

    return m_EntryInfo.m_file_index;
}

uint64_t Impl::ZipEntry::CompressedSize() const {

    return m_EntryInfo.m_comp_size;
}

uint64_t Impl::ZipEntry::UncompressedSize() const {

    return m_EntryInfo.m_uncomp_size;
}

bool Impl::ZipEntry::IsDirectory() const {

    return m_EntryInfo.m_is_directory;
}

bool Impl::ZipEntry::IsEncrypted() const {

    return m_EntryInfo.m_is_encrypted;
}

bool Impl::ZipEntry::IsSupported() const {

    return m_EntryInfo.m_is_supported;
}

std::string Impl::ZipEntry::Filename() const {

    return m_EntryInfo.m_filename;
}

std::string Impl::ZipEntry::Comment() const {

    return m_EntryInfo.m_comment;
}

const time_t& Impl::ZipEntry::Time() const {

    return m_EntryInfo.m_time;
}

bool Impl::ZipEntry::IsModified() const {

    return m_IsModified;
}

uint32_t Impl::ZipEntry::GetNewIndex() {

    return ++s_LatestIndex;
}

Zippy::ZipEntryInfo Impl::ZipEntry::CreateInfo(const std::string& name) {

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

ZipEntry::ZipEntry(Impl::ZipEntry* entry)
        : m_ZipEntry(entry) {

}

ZipEntryData ZipEntry::GetData() const {

    return m_ZipEntry->GetData();
}

std::string ZipEntry::GetDataAsString() const {

    return m_ZipEntry->GetDataAsString();
}

void ZipEntry::SetData(const std::string& data) {

    m_ZipEntry->SetData(data);
}

void ZipEntry::SetData(const ZipEntryData& data) {

    m_ZipEntry->SetData(data);
}

uint32_t ZipEntry::Index() const {

    return m_ZipEntry->Index();
}

uint64_t ZipEntry::CompressedSize() const {

    return m_ZipEntry->CompressedSize();
}

uint64_t ZipEntry::UncompressedSize() const {

    return m_ZipEntry->UncompressedSize();
}

bool ZipEntry::IsDirectory() const {

    return m_ZipEntry->IsDirectory();
}

bool ZipEntry::IsEncrypted() const {

    return m_ZipEntry->IsEncrypted();
}

bool ZipEntry::IsSupported() const {

    return m_ZipEntry->IsSupported();
}

std::string ZipEntry::Filename() const {

    return m_ZipEntry->Filename();
}

std::string ZipEntry::Comment() const {

    return m_ZipEntry->Comment();
}

const time_t& ZipEntry::Time() const {

    return m_ZipEntry->Time();
}

bool ZipEntry::IsModified() const {

    return m_ZipEntry->IsModified();
}
