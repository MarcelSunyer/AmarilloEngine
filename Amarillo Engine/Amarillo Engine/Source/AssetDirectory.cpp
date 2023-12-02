#include "AssetDirectory.h"
#include "Define_Resource.h"
#include "ResourceManager.h"

AssetDir::AssetDir(const char* _dName, const char* _imPath, uint64 _lMod, bool _dir) : isDir(_dir), lastModTime(_lMod)
{
	dirName = _dName;
	dirName.push_back('\0');
	importPath = _imPath;
	importPath.push_back('\0');

	if (!isDir)
	{
		GenerateMetaPath();
		/*if (HasMeta())
		{
			metaUID = applic->resourceManager->GetMetaUID(metaFileDir.c_str());
		}*/
	}
}

AssetDir::~AssetDir()
{
	dirName.clear();
	childDirs.clear();
	importPath.clear();
	metaFileDir.clear();
	lastModTime = 0;
}

void AssetDir::ClearData()
{
	dirName.clear();
	childDirs.clear();
	isDir = false;
	importPath.clear();
	metaFileDir.clear();
	lastModTime = 0;
}



void AssetDir::GenerateMeta()
{
	GenerateMetaPath();

	/*if (!HasMeta())
	{
		Resource::Type type = applic->resourceManager->GetTypeFromAssetExtension(importPath.c_str());
		uint resUID = applic->resourceManager->GenerateNewUID();
		metaUID = resUID;
		applic->resourceManager->GenerateMeta(importPath.c_str(), applic->resourceManager->GenLibraryPath(resUID, type).c_str(), resUID, type);
	}*/
}

void AssetDir::GenerateMetaRecursive()
{
	if (!isDir)
	{
		GenerateMeta();
	}

	for (size_t i = 0; i < childDirs.size(); i++)
	{
		childDirs[i].GenerateMetaRecursive();
	}
}

void AssetDir::CreateLibraryFileRecursive()
{
	//Create library file
	if (!isDir && applic->resourceManager->ExistsOnLibrary(importPath.c_str()) == 0) 
	{
		//Create mirror
		uint id = applic->resourceManager->CreateLibraryFromAssets(importPath.c_str());
	}

	for (size_t i = 0; i < childDirs.size(); i++)
	{
		childDirs[i].CreateLibraryFileRecursive();
	}
}

void AssetDir::GenerateMetaPath()
{
	metaFileDir = importPath;
	metaFileDir.pop_back();

	metaFileDir += ".meta";
	metaFileDir.push_back('\0');
}

void AssetDir::DeletePermanent()
{
	//Directory can only be deleted if there is nothing inside
	if (isDir && childDirs.size() != 0)
	{
		//Delete all files inside recursive
		for (size_t i = 0; i < childDirs.size(); i++)
		{
			childDirs[i].DeletePermanent();
		}
	}

	//Remove library
	if (!isDir)
	{
		//applic->moduleFileSystem->DeleteAssetFile(applic->resourceManager->LibraryFromMeta(this->metaFileDir.c_str()).c_str());

		if (applic->resourceManager->GetMetaType(metaFileDir.c_str()) == Resource::Type::MESH)
		{
			std::vector<uint> meshesID;
			//Get meshes from fbx meta
			//if (FileSystem::Exists(metaFileDir.c_str()))
			//{
			//	ModelImporter::GetMeshesFromMeta(importPath.c_str(), meshesID);

			//	//Delete said meshes
			//	for (int i = 0; i < meshesID.size(); i++)
			//	{
			//		std::string meshes = meshes;
			//		meshes += std::to_string(meshesID[i]);
			//		meshes += ".mmh";
			//		applic->moduleFileSystem->DeleteAssetFile(meshes.c_str());
			//	}

			//	applic->resourceManager->UpdateMeshesDisplay();
			//}
		}
	}

	//applic->moduleFileSystem->DeleteAssetFile(importPath.c_str());
	//applic->moduleFileSystem->DeleteAssetFile(metaFileDir.c_str());


	ClearData();
}