#ifndef __MODULE_FILE_SYSTEM_H__
#define __MODULE_FILE_SYSTEM_H__

#include "Module.h"
#include <filesystem>
#include <functional>


struct DecomposedFilePath
{
	std::string file_name;
	std::string file_extension;
	std::string file_extension_lower_case;
	std::string path;
	std::string file_path;
};

class Folder
{
public:
	std::vector<DecomposedFilePath> files;
	std::string folder_path;
	std::string folder_name;
	std::vector<Folder> folders;

	bool valid = false;
};

class WatchingFloder
{
public:
	std::string folder;
	std::filesystem::file_time_type last_time;
};

class FileSystem : public Module
{
public:
	FileSystem(Application* app, bool start_enabled = true);
	~FileSystem();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	std::string CreateFolder(const char* path, const char* name);

	bool FileCopyPaste(std::filesystem::path filepath, std::filesystem::path new_path, bool overwrite);
	void FileCopyPasteWithNewName(const char* filepath, const char* new_path, const char* new_name);
	bool FileDelete(const char* filepath);
	bool FolderDelete(const char* folderpath);
	bool FileSave(const char* path, const char* file_content, const char* name, const char* extension, int size);
	bool FileSave(std::filesystem::path path, const char* file_content, int size);
	std::vector<char> LoadFile(std::filesystem::path path);
	
	std::vector<std::string> GetFilesAndFoldersInPath(const char* path, const char* extension = "");
	std::vector<std::string> GetFoldersInPath(const char* path);
	std::vector<std::filesystem::path> GetFilesFromFolder(std::filesystem::path folder, bool recursive);

	bool FileExists(std::filesystem::path path);
	bool FileRename(const char* filepath, const char* new_name);
	bool FolderRename(const char* filepath, const char* new_name);
	std::string FileRenameOnNameCollision(const char* path, const char* name, const char* extension);

	//Useful fuctions
	std::string ToLowerCase(std::string str);
	bool TextCmp(const char* text1, const char* text2);

	std::string GetAssetsPath();
	std::string GetLibraryPath();
	std::string GetLibraryMeshPath();
	std::string GetLibraryPrefabPath();
	std::string GetLibraryTexturePath();
	std::string GetLibraryScenePath();
	std::string GetLibraryShadersPath();
	std::string GetSettingsPath();

	std::string GetLookingPath();
	void SetLookingPath(const std::string& new_path);

	DecomposedFilePath DecomposeFilePath(std::string file_path);

	// DEPRECATED ---------------------------------------------------

	// Example file.ex -> .ex
	std::string GetFileExtension(const char* file_name);

	// Example file.ex -> file
	std::string GetFilenameWithoutExtension(const char* file_name);

	// Example C:/user/folder/file.ex -> file.ex
	std::string GetFileNameFromFilePath(const char* file_path);

	// Example C:/user/folder/file.ex -> C:/user/folder/
	std::string GetPathFromFilePath(const char* file_path);

	// Example C:/user/folder/ -> folder
	std::string GetFolderNameFromPath(const char* path);

	// Example C:/user/folder/ -> C:/user/
	std::string GetParentFolder(const char* folder_path);

private:
	// Same name file renaming
	std::string NewNameForFileNameCollision(const char* filename);
	int GetFileNameNumber(const char* filename);
	std::string SetFileNameNumber(const char* filename, int number);

private:
	std::string assets_path;
	std::string library_path;
	std::string library_mesh_path;
	std::string library_prefab_path;
	std::string library_texture_path;
	std::string library_scene_path;
	std::string library_shaders_path;
	std::string settings_path;

	std::string looking_path;

	std::vector<WatchingFloder> watching_folders;
};

void Changed(const std::filesystem::path& path);

#endif // __MODULE_FILE_SYSTEM_H__