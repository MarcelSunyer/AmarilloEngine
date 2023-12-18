#include "ModuleFileSystem.h"
#include "Application.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <ctype.h>

FileSystem::FileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	assets_path = CreateFolder(App->GetBasePath(), "Assets");
	library_path = CreateFolder(App->GetBasePath(), "Library");
	library_mesh_path = CreateFolder(library_path.c_str(), "Meshes");
	library_prefab_path = CreateFolder(library_path.c_str(), "Prefabs");
	library_texture_path = CreateFolder(library_path.c_str(), "Textures");
	library_scene_path = CreateFolder(library_path.c_str(), "Scenes");
	library_shaders_path = CreateFolder(library_path.c_str(), "Shades");
	settings_path = CreateFolder(App->GetBasePath(), "Settings");
}

FileSystem::~FileSystem()
{
}

bool FileSystem::Awake()
{
	bool ret = true;

	return ret;
}

bool FileSystem::Start()
{
	bool ret = true;

	return ret;
}

bool FileSystem::Update()
{
	bool ret = true;

	//for (std::vector<WatchingFloder>::iterator it = watching_folders.begin(); it != watching_folders.end(); ++it)
	//{
	//	std::experimental::filesystem::file_time_type time_type = std::experimental::filesystem::last_write_time((*it).folder);
	//	std::time_t curr_time = decltype(time_type)::clock::to_time_t(time_type);

	//	std::time_t last_time = decltype((*it).last_time)::clock::to_time_t((*it).last_time);

	//	if (curr_time > last_time)
	//	{
	//		int i = 0;
	//		++i;
	//	}

	//}

	return ret;
}

bool FileSystem::CleanUp()
{
	bool ret = true;

	return ret;
}

std::string FileSystem::GetAssetsPath()
{
	return assets_path;
}

std::string FileSystem::GetLibraryPath()
{
	return library_path;
}

std::string FileSystem::GetLibraryMeshPath()
{
	return library_mesh_path;
}

std::string FileSystem::GetLibraryPrefabPath()
{
	return library_prefab_path;
}

std::string FileSystem::GetLibraryTexturePath()
{
	return library_texture_path;
}

std::string FileSystem::GetLibraryScenePath()
{
	return library_scene_path;
}

std::string FileSystem::GetLibraryShadersPath()
{
	return library_shaders_path;
}

std::string FileSystem::GetSettingsPath()
{
	return settings_path;
}

std::string FileSystem::GetLookingPath()
{
	return looking_path;
}

void FileSystem::SetLookingPath(const std::string& new_path)
{
	looking_path = new_path;
}

DecomposedFilePath FileSystem::DecomposeFilePath(std::string file_path)
{
	DecomposedFilePath ret;

	bool adding_file_extension = false;
	bool adding_file_name = false;
	int last_bar_pos = 0;

	for (int i = 0; i < file_path.length(); ++i)
	{
		char curr_word = file_path[i];

		// Formating --------------------

		if (curr_word == '/')
			curr_word = '\\';

		ret.file_path += curr_word;

		// ------------------------------

		// File extension ---------------
		if (adding_file_extension)
			ret.file_extension += curr_word;

		if (curr_word == '.')
		{
			if (adding_file_extension && ret.file_extension.size() > 1)
			{
				std::string to_append = "." + ret.file_extension.substr(0, ret.file_extension.size() - 1);
				ret.file_name.append(to_append);
			}

			adding_file_extension = true;
			ret.file_extension.clear();
		}
		// -------------------------------

		// File name ---------------------

		if (curr_word == '.')
		{
			adding_file_name = false;
		}

		if (adding_file_name)
			ret.file_name += curr_word;

		if (curr_word == '\\')
		{
			last_bar_pos = i;
			adding_file_name = true;
			ret.file_name.clear();
		}

		// -------------------------------
	}

	ret.file_extension_lower_case = ToLowerCase(ret.file_extension);

	// Path ---------------------

	for (int i = 0; i <= last_bar_pos; ++i)
	{
		ret.path += ret.file_path[i];
	}

	// --------------------------

	return ret;
}

std::string FileSystem::NewNameForFileNameCollision(const char* filename)
{
	std::string ret;

	int number = GetFileNameNumber(filename);

	if (number != -1)
	{
		ret = SetFileNameNumber(filename, number + 1);
	}
	else
	{
		ret = SetFileNameNumber(filename, 1);
	}

	return ret;
}

int FileSystem::GetFileNameNumber(const char* filename)
{
	int ret = -1;

	std::string name = filename;

	std::string number_str;
	bool adding = false;

	for (int i = 0; i < name.size(); ++i)
	{
		if (name[i] == '(')
		{
			adding = true;
			number_str.clear();
			continue;
		}

		if (name[i] == ')')
		{
			adding = false;
		}

		if (adding)
		{
			if (!isdigit(name[i]))
			{
				number_str.clear();
				adding = false;
				continue;
			}

			number_str += name[i];
		}
	}

	if (number_str.size() > 0)
	{
		ret = atoi(number_str.c_str());
	}

	return ret;
}

std::string FileSystem::SetFileNameNumber(const char* filename, int number)
{
	std::string ret = filename;

	if (GetFileNameNumber(filename) != -1)
	{
		int start = 0;
		bool has_start = false;
		bool has_end = false;
		int end = 0;

		for (int i = ret.size() - 1; i >= 0; --i)
		{
			if (ret[i] == ')')
			{
				start = i;
				has_start = true;
			}

			if (ret[i] == '(' && has_start)
			{
				end = i;
				has_end = true;
				break;
			}
		}

		if (has_start && has_end)
			ret = ret.substr(0, end);
	}

	ret += ('(' + std::string("%d", number) + ')');

	return ret;
}

std::string FileSystem::GetFileExtension(const char* file_name)
{
	std::string ret;

	bool adding = false;
	for (int i = 0; file_name[i] != '\0'; ++i)
	{
		if (file_name[i] == '.')
		{
			ret.clear();
			adding = true;
			continue;
		}

		if (adding)
			ret += file_name[i];
	}

	return ret;
}

std::string FileSystem::GetFilenameWithoutExtension(const char* file_name)
{
	std::string ret;

	for (int i = 0; file_name[i] != '\0'; ++i)
	{
		if (file_name[i] == '.')
		{
			break;
		}

		ret += file_name[i];
	}

	return ret;
}

std::string FileSystem::GetFileNameFromFilePath(const char* file_path)
{
	std::string ret;

	for (int i = 0; file_path[i] != '\0'; ++i)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			ret.clear();
			continue;
		}

		ret += file_path[i];
	}

	return ret;
}

std::string FileSystem::GetPathFromFilePath(const char* file_path)
{
	std::string ret;

	int last = 0;
	for (int i = 0; file_path[i] != '\0'; ++i)
	{
		if (file_path[i] == '\\')
		{
			last = i;
			last++;
		}
	}

	for (int i = 0; i < last && file_path[i] != '\0'; ++i)
	{
		ret += file_path[i];
	}

	return ret;
}

std::string FileSystem::GetFolderNameFromPath(const char* path)
{
	std::string s_path = path;

	std::string ret;

	for (int i = 0; i < s_path.size(); ++i)
	{
		ret += s_path[i];

		if (s_path[i] == '\\' && s_path.size() - 1 != i)
		{
			ret.clear();
		}
	}

	if (ret.size() > 0)
	{
		if (ret[ret.size() - 1] == '\\')
		{
			ret = ret.substr(0, ret.size() - 1);
		}
	}

	return ret;
}

std::string FileSystem::GetParentFolder(const char* folder_path)
{
	std::string s_path = folder_path;

	std::string ret;

	std::string curr_folder;
	for (int i = 0; i < s_path.size(); ++i)
	{
		curr_folder += s_path[i];

		if (s_path[i] == '\\')
		{
			if (i + 1 < s_path.size())
			{
				ret += curr_folder;
				curr_folder.clear();
			}
		}
	}

	return ret;
}

std::string FileSystem::CreateFolder(const char* path, const char* name)
{
	std::string ret;

	std::string filepath = path;

	if (filepath[filepath.length() - 1] != '\\')
	{
		filepath += '\\';
	}

	filepath += name;

	DWORD error = GetLastError();

	if (CreateDirectory(filepath.c_str(), NULL) == 0)
	{
		error = GetLastError();
	}

	if (error == ERROR_PATH_NOT_FOUND)
	{
		LOG("Error creating folder (path not found): %s", path);
		return ret;
	}
	else if (error == ERROR_ALREADY_EXISTS)
	{
		LOG("Error creating folder (Folder aleady exists): %s", filepath.c_str())
	}

	ret = filepath + '\\';

	return ret;
}

bool FileSystem::FileCopyPaste(std::filesystem::path filepath, std::filesystem::path new_path, bool overwrite = false)
{
	std::filesystem::copy_options options = std::filesystem::copy_options::recursive;

	if (overwrite)
	{
		options |= std::filesystem::copy_options::overwrite_existing;
	}

	try 
	{
		std::filesystem::copy(filepath, new_path, options);
	}
	catch (const std::filesystem::filesystem_error& e) 
	{
		return false;
	}

	return true;
}

void FileSystem::FileCopyPasteWithNewName(const char* filepath, const char* new_path, const char* new_name)
{
	DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

	std::string changed_original_filepath = d_filepath.path + new_name + "." + d_filepath.file_extension;

	if (FileRename(filepath, new_name))
	{
		FileCopyPaste(changed_original_filepath.c_str(), new_path, false);

		FileRename(changed_original_filepath.c_str(), d_filepath.file_name.c_str());
	}
}

bool FileSystem::FileDelete(const char* filepath)
{
	bool ret = false;

	if (DeleteFile(filepath) == 0)
	{
		DWORD error = GetLastError();

		if (error == ERROR_FILE_NOT_FOUND)
		{
			LOG("Error deleting file (path not found)): %s", filepath);
		}
	}
	else
		ret = true;

	return ret;
}

bool FileSystem::FolderDelete(const char* folderpath)
{
	bool ret = false;

	if (RemoveDirectory(folderpath) != 0)
		ret = true;
	else
	{
		LOG("Error deleting path): %s", folderpath);
	}

	return ret;
}

bool FileSystem::FileSave(const char* path, const char* file_content, const char* name, const char* extension, int size)
{
	bool ret = false;

	std::string file = path;
	file += name;
	file += ".";
	file += extension;

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else
	{
		LOG("Error saving file %s: ", name);
	}

	if (new_file)
	{
		fclose(new_file);
	}

	return ret;
}

bool FileSystem::FileSave(std::filesystem::path path, const char* file_content, int size)
{
	bool ret = false;

	const char* path_c = path.string().c_str();

	std::ofstream outputFile(path, std::ios::binary);

	if (outputFile.is_open())
	{
		outputFile.write(file_content, size);
		outputFile.close();
		ret = true;
	}
	else
	{
		LOG("Error saving file %s: ", path_c);
	}

	return ret;
}

std::vector<char> FileSystem::LoadFile(std::filesystem::path path)
{
	if (std::filesystem::exists(path))
	{
		std::ifstream inputFile(path, std::ios::binary);

		if (inputFile.is_open())
		{
			inputFile.seekg(0, std::ios::end);
			std::streampos fileSize = inputFile.tellg();
			inputFile.seekg(0, std::ios::beg);

			std::vector<char> fileContents(static_cast<size_t>(fileSize));
			inputFile.read(fileContents.data(), fileSize);
			return fileContents;
		}
	}

	return std::vector<char>();
}

std::vector<std::string> FileSystem::GetFilesAndFoldersInPath(const char* path, const char* extension)
{
	std::string s_path = path;

	if (s_path[s_path.length() - 1] != '\\')
	{
		s_path += '\\';
	}

	std::vector<std::string> files;

	WIN32_FIND_DATA search_data;

	std::string path_ex = s_path;

	if (!TextCmp(extension, ""))
	{
		path_ex += "*.";
		path_ex += extension;
	}
	else
	{
		path_ex += "*.*";
	}

	HANDLE handle = FindFirstFile(path_ex.c_str(), &search_data);

	int counter = 0;
	while (handle != INVALID_HANDLE_VALUE)
	{
		bool can_add = true;
		if ((search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (TextCmp("..", search_data.cFileName) || TextCmp(".", search_data.cFileName))
			{
				can_add = false;
			}
		}

		if (can_add)
		{
			std::string path_new = s_path;
			path_new += search_data.cFileName;

			if ((search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				path_new += "\\";

			files.push_back(path_new);
		}

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	if (handle)
		FindClose(handle);

	return files;
}

std::vector<std::string> FileSystem::GetFoldersInPath(const char* path)
{
	std::string s_path = path;

	if (s_path[s_path.length() - 1] != '\\')
	{
		s_path += '\\';
	}

	std::vector<std::string> files;

	WIN32_FIND_DATA search_data;

	std::string path_ex = s_path;
	path_ex += "*.*";

	HANDLE handle = FindFirstFile(path_ex.c_str(), &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		if ((search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (!TextCmp("..", search_data.cFileName) && !TextCmp(".", search_data.cFileName))
			{
				std::string path_new = s_path;
				path_new += search_data.cFileName + std::string("\\");
				files.push_back(path_new);
			}
		}

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	if (handle)
		FindClose(handle);

	return files;
}

std::vector<std::filesystem::path> FileSystem::GetFilesFromFolder(std::filesystem::path folder, bool recursive)
{
	std::vector<std::filesystem::path> filespaths;
	
	if (!recursive)
	{
		for (const auto& entry : std::filesystem::directory_iterator(folder))
		{
			if (std::filesystem::is_regular_file(entry.path()))
			{
				filespaths.push_back(entry.path());
			}
		}
	}
	else 
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(folder))
		{
			if (std::filesystem::is_regular_file(entry.path()))
			{
				filespaths.push_back(entry.path());
			}
		}
	}

	return filespaths;
}



bool FileSystem::FileExists(std::filesystem::path path)
{
	return std::filesystem::exists(path);
}

bool FileSystem::FileRename(const char* filepath, const char* new_name)
{
	bool ret = false;

	DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

	std::string new_filepath = d_filepath.path + new_name + "." + d_filepath.file_extension;

	if (rename(filepath, new_filepath.c_str()) == 0)
		ret = true;

	return ret;
}

bool FileSystem::FolderRename(const char* folderpath, const char* new_name)
{
	std::string parent_folder = GetParentFolder(folderpath);

	std::string new_path = parent_folder + new_name + '\\';
	if (MoveFileEx(folderpath, new_path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) == 0)
	{
		DWORD error = GetLastError();
		if (error != 0)
			LOG("Error renaming folder:[%s] to [%s]", folderpath, new_path.c_str())
	}

	return false;
}



std::string FileSystem::FileRenameOnNameCollision(const char* path, const char* name, const char* extension)
{
	std::string ret;

	std::string s_path = path;
	std::string s_name = name;
	std::string s_extension = extension;

	ret = s_name;

	std::string new_filepath = s_path + s_name + "." + std::string(extension);

	bool need_rename = false;
	while (FileExists(new_filepath.c_str()))
	{
		std::string check_new_name = NewNameForFileNameCollision(s_name.c_str());
		s_name = check_new_name;

		new_filepath = s_path + s_name + "." + s_extension;

		need_rename = true;
	}

	if (need_rename)
		ret = s_name;

	return ret;
}

void Changed(const std::filesystem::path& path)
{
	LOG(" -----------------------enter-----------------------");
	int i = 0;
	++i;
}


//Usefull fuctions
std::string FileSystem::ToLowerCase(std::string str)
{
	for (uint i = 0; i < str.size(); ++i)
	{
		str[i] = tolower(str[i]);
	}

	return str;
}

bool FileSystem::TextCmp(const char* text1, const char* text2)
{
	bool ret = false;

	if (text1 == nullptr || text2 == nullptr)
		return false;

	if (strcmp(text1, text2) == 0)
		ret = true;

	return ret;
}