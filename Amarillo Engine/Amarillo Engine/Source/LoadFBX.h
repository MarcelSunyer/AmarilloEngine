#include "..\External\Assimp/include/cimport.h"
#include "..\External\Assimp/include/scene.h"
#include "..\External\Assimp/include/postprocess.h"

#pragma comment (lib, "External/Assimp/libx86/assimp.lib")

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleConsole.h"
#include "Mesh.h"
#include <vector>
#include <string>
#include "Module.h"
#include "..\External\MathGeoLib/include/Math/float3.h"
#include "..\External\MathGeoLib/include/Math/float2.h" // Agregado para float2

///

#include "..\External\Assimp/include/mesh.h"
#include "..\External\Assimp/include/cimport.h"
#include "..\External\Assimp/include/scene.h"
#include "..\External\Assimp/include/postprocess.h"

#pragma comment (lib, "External/Assimp/libx86/assimp.lib")

#include "..\External\Devil/Include/ilut.h"
#include "..\External\Devil/Include/il.h"
#include "..\External\Devil/include/ilut.h"

#pragma comment (lib, "External/Devil/libx86/DevIL.lib")
#pragma comment (lib, "External/Devil/libx86/ILU.lib")
#pragma comment (lib, "External/Devil/libx86/ILUT.lib")

///

class LoadFBX
{
public:
    LoadFBX(Application* app);
    ~LoadFBX();

    void Load(const char* path)
    {
        loadModel(path);
    }

    void Draw()
    {
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, textureID); // Vincular la textura
        
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw();
    }

    uint TextureImport(const char* path)
    {
        uint Il_Tex;
        uint tempid;
        ilGenImages(1, &Il_Tex);
        ilBindImage(Il_Tex);
        ilLoadImage(path);
        tempid = ilutGLBindTexImage();
        ilDeleteImages(1, &Il_Tex);
        if (Il_Tex != NULL)
        {
            LOG("Successfully loaded %s texture", path);
        }
        else {
            LOG("Error loading the texture!");
        }
        return tempid;
    }

    bool isLoaded = false;
    bool error = false;

private:
    Application* App;
    // model data
    std::vector<Mesh> meshes;
    uint textureID; // ID de la textura

    void loadModel(const char* file_path)
    {
        const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            LOG("Error loading scene: %s", file_path);
            this->error = true;
            return;
        }
        else LOG("Scene loaded successfully: %s", file_path);
        this->isLoaded = true;
        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.Position = float3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            if (mesh->HasNormals())
            {
                vertex.Normal = float3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }

            if (mesh->mTextureCoords[0])
            {
                vertex.TexCoords = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            else
            {
                vertex.TexCoords = float2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        LOG("Num Vertex: %d", mesh->mNumVertices);
        LOG("Num Index: %d", mesh->mNumFaces);
        return Mesh(vertices, indices);
    }
};
