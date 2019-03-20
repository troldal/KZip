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
     *       Zippy::ZipArchive arch;
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
    class ZipArchive {

    public:

        /**
         * @brief Constructor. Constructs a null-archive, which can be used for creating a new .zip file
         * or opening an existing one.
         * @warning Before using an archive object that has been default constructed, a call to either Open() or Create() must be
         * performed. Otherwise, the object will be in a null-state and calls to member functions will be undefined.
         */
        explicit ZipArchive() = default;

        /**
         * @brief Constructor. Constructs an archive object, using the fileName input parameter. If the file already exists,
         * it will be opened. Otherwise, a new object will be created.
         * @details
         * #### Implementation details
         * The constructors tries to open an std::ifstream. If it is valid, it means that a file already exists
         * and will be opened. Otherwise, the file does not exist and will be created.
         * @param fileName The name of the file to open or create.
         */
        explicit ZipArchive(const std::string& fileName);

        /**
         * @brief Copy Constructor (deleted).
         * @param other The object to copy
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
        virtual ~ZipArchive();

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
        void Create(const std::string& fileName);

        /**
         * @brief Open an existing archive file with the given filename.
         * @details
         * ##### Implementation details
         * The archive file is opened and meta data for all the entries in the archive is loaded into memory.
         * @param fileName The filename of the archive to open.
         * @note If more than one entry with the same name exists in the archive, only the newest one will be loaded.
         * When saving the archive, only the loaded entries will be kept; other entries with the same name will be deleted.
         */
        void Open(const std::string& fileName);

        /**
         * @brief Close the archive for reading and writing.
         * @note If the archive has been modified but not saved, all changes will be discarded.
         */
        void Close();

        /**
         * @brief Checks if the archive file is open for reading and writing.
         * @return true if it is open; otherwise false;
         */
        bool IsOpen();

        /**
         * @brief Get a list of the entries in the archive. Depending on the input parameters, the list will include
         * directories, files or both.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of std::strings with the entry names.
         */
        std::vector<std::string> GetEntryNames(bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief Get a list of the entries in a specific directory of the archive. Depending on the input parameters,
         * the list will include directories, files or both.
         * @param dir The directory with the entries to get.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of std::strings with the entry names. The directory itself is not included.
         */
        std::vector<std::string> GetEntryNamesInDir(const std::string& dir, bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief Get a list of the metadata for entries in the archive. Depending on the input parameters, the list will include
         * directories, files or both.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of ZipEntryMetaData structs with the entry metadata.
         */
        std::vector<ZipEntryMetaData> GetMetaData(bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief Get a list of the metadata for entries in a specific directory of the archive. Depending on the input
         * parameters, the list will include directories, files or both.
         * @param dir The directory with the entries to get.
         * @param includeDirs If true, the list will include directories; otherwise not. Default is true
         * @param includeFiles If true, the list will include files; otherwise not. Default is true
         * @return A std::vector of ZipEntryMetaData structs with the entry metadata. The directory itself is not included.
         */
        std::vector<ZipEntryMetaData> GetMetaDataInDir(const std::string& dir, bool includeDirs = true, bool includeFiles = true);
        /**
         * @brief Get the number of entries in the archive. Depending on the input parameters, the number will include
         * directories, files or both.
         * @param includeDirs If true, the number will include directories; otherwise not. Default is true
         * @param includeFiles If true, the number will include files; otherwise not. Default is true
         * @return An int with the number of entries.
         */
        int GetNumEntries(bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief Get the number of entries in a specific directory of the archive. Depending on the input parameters,
         * the list will include directories, files or both.
         * @param dir The directory with the entries to get.
         * @param includeDirs If true, the number will include directories; otherwise not. Default is true.
         * @param includeFiles If true, the number will include files; otherwise not. Default is true.
         * @return An int with the number of entries. The directory itself is not included.
         */
        int GetNumEntriesInDir(const std::string& dir, bool includeDirs = true, bool includeFiles = true);

        /**
         * @brief Check if an entry with a given name exists in the archive.
         * @param entryName The name of the entry to check for.
         * @return true if it exists; otherwise false;
         */
        bool HasEntry(const std::string& entryName);

        /**
         * @brief Save the archive. This will apply all modifications and cannot be undone.
         */
        void Save();

        /**
         * @brief Save the archive with a new name. The original archive will remain unchanged.
         * @param filename The new filename.
         */
        void SaveAs(std::string filename);

        /**
         * @brief Deletes an entry from the archive.
         * @param name The name of the entry to delete.
         */
        void DeleteEntry(const std::string& name);

        /**
         * @brief Get the entry with the specified name.
         * @param name The name of the entry in the archive.
         * @return A ZipEntry object with the requested entry.
         */
        ZipEntry GetEntry(const std::string& name);

        /**
         * @brief Add a new entry to the archive.
         * @param name The name of the entry to add.
         * @param data The ZipEntryData to add. This is an alias of a std::vector<std::byte>, holding the binary data of the entry.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
        ZipEntry AddEntry(const std::string& name, const ZipEntryData& data);

        /**
         * @brief Add a new entry to the archive.
         * @param name The name of the entry to add.
         * @param data The std::string data to add.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
        ZipEntry AddEntry(const std::string& name, const std::string& data);

        /**
         * @brief Add a new entry to the archive, using another entry as input. This is mostly used for cloning existing entries
         * in the same archive, or for copying entries from one archive to another.
         * @param name The name of the entry to add.
         * @param entry The ZipEntry to add.
         * @return The ZipEntry object that has been added to the archive.
         * @note If an entry with given name already exists, it will be overwritten.
         */
        ZipEntry AddEntry(const std::string& name, const ZipEntry& entry);

    private:

        /**
         * @brief Throws the correct exception based on the error code, and adds a description.
         * @param error The miniz errorcode
         * @param errorString A description of the error.
         */
        static void ThrowException(mz_zip_error error, const std::string& errorString);

        /**
         * @brief Generates a random filename, which is used to generate a temporary archive when modifying and saving
         * archive files.
         * @param length The length of the filename to create.
         * @return Returns the generated filenamen, appended with '.tmp'.
         */
        static std::string GenerateRandomName(int length);

    private:
        mz_zip_archive m_Archive     = mz_zip_archive(); /**< The struct used by miniz, to handle archive files. */
        std::string    m_ArchivePath = ""; /**< The path of the archive file. */
        bool           m_IsOpen      = false; /**< A flag indicating if the file is currently open for reading and writing. */

        std::vector<Impl::ZipEntry> m_ZipEntries = std::vector<Impl::ZipEntry>(); /**< Data structure for all entries in the archive. */

    };
}  // namespace Zippy

#endif //Zippy_ZIPARCHIVE_H
