#include "..\External\Assimp/include/cimport.h"
#include "..\External\Assimp/include/scene.h"
#include "..\External\Assimp/include/postprocess.h"

#pragma comment (lib, "External/Assimp/libx86/assimp.lib")

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleConsole.h"
#include "AssimpNamespace.h"
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

    void Load(const char* path, GameObject* gameobject)
    {
        loadModel(path);
        if (!isLoaded || error)
        {
            // Manejar el error, si es necesario
            return;
        }
        Meshes* fbxMesh = fbxLoader->GetLoadedMesh();

        if (gameobject)
        {
            ComponentMesh* meshComponent = (ComponentMesh*)gameobject->AddComponent(new ComponentMesh(gameobject));
            if (meshComponent)
            {
                meshComponent->SetMesh(fbxMesh);
                meshComponent->SetPath(path);
            }
        }
    }
  
    void Draw()
    {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);

        //Test texturas
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindTexture(GL_TEXTURE_2D, textureID); // Vincular la textura
        
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
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

    void DrawNormals()
    {
        glColor3f(1.0f, 1.0f, 0.0f);
        glLineWidth(2.0f);  // Set the line width for the normals

        for (const Mesh& mesh : meshes)
        {
            for (unsigned int i = 0; i < mesh.indices.size(); i += 3)
            {
                // Get the vertices of the triangle
                float3 v0 = mesh.vertices[mesh.indices[i + 0]].Position;
                float3 v1 = mesh.vertices[mesh.indices[i + 1]].Position;
                float3 v2 = mesh.vertices[mesh.indices[i + 2]].Position;

                // Calculate the center of the triangle
                float3 center = (v0 + v1 + v2) / 3.0f;

                // Calculate the normal of the triangle
                float3 normal = Cross(v1 - v0, v2 - v0).Normalized();

                // Calculate the start and end positions of the normal line
                float3 startPos = center;
                float3 endPos = center + normal * 0.1f;  // Scale the normal length for visibility

                // Draw the normal line
                glBegin(GL_LINES);
                glVertex3f(startPos.x, startPos.y, startPos.z);
                glVertex3f(endPos.x, endPos.y, endPos.z);
                glEnd();
            }
        }
    }


    bool isLoaded = false;
    bool error = false;

    uint textureID; // ID de la textura
private:
    Application* App;
    // model data
    std::vector<Mesh> meshes;
    Meshes loadedMesh;
    
    Meshes* LoadFBX::GetLoadedMesh()
    {
        return &loadedMesh;
    }

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
