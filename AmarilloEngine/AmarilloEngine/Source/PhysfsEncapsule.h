#pragma once

#include <cstddef>
#include <string>
#include <fstream>
#include <filesystem>

#include "Globals.h"


#include "../External/PhysFS/include/physfs.h"
#pragma comment (lib, "Source/External/PhysFS/libx86/physfs.lib")

namespace PhysfsEncapsule {

    void InitializePhysFS();

    bool FolderExists(std::string route);
    bool FileExists(std::string route);
    void CreateFolder(std::string route, std::string folderName);
    void ReadFolder(std::string route);

    // Delete file or folder
    void DeleteFS(std::string route);

    bool CopyFileFromSource(std::string source, std::string destination);   

    void DeinitializePhysFS();
    
    //
    // Save
    uint SaveFile(const char* file, const void* buffer, uint size, bool append = false);

    // Load
	//// Open for Read/Write
    uint LoadFileToBuffer(const char* file, char** buffer);

	// Getters
    bool IsDirectory(const char* file);
    void DiscoverFiles(const char* directory, std::vector<std::string>& vFiles, std::vector<std::string>& vDirs);

    //
    std::string GetUniqueName(const char* path, const char* name);
    void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr);
   
    // Check extension
    bool HasExtension(const char* path);
    //// Compare extension with a given one
    bool HasExtension(const char* path, std::string extension);
    //// Check if has one of x extension
    bool HasExtension(const char* path, std::vector<std::string> extensions);

    // Duplicate
    bool DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath);
    bool DuplicateFile(const char* srcFile, const char* dstFile);

    // Path handling
    std::string UnNormalizePath(const char* full_path);
    std::string NormalizePath(const char* full_path);

    uint LoadToBuffer(const char* file, char** buffer);
    //Rename
    bool RenameFile(std::string oldFile, std::string newFile);

}