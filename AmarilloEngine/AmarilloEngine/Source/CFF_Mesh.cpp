#include "CFF_Mesh.h"

void CFF_Mesh::Import(const aiMesh* mesh, ModuleMesh::Mesh* ourMesh)
{

}

uint CFF_Mesh::Save(const ModuleMesh::Mesh* ourMesh, char** fileBuffer)
{
    // Define the size of the header (ranges) and data (vertices)
    uint headerSize = sizeof(uint) * 2; // num_indices and num_vertices
    uint vertexSize = sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 2; // size of Vertex structure

    // Calculate the total size of the file buffer
    uint size = headerSize + sizeof(uint) * ourMesh->indices.size() + vertexSize * ourMesh->ourVertex.size();

    // Allocate memory for the file buffer
    *fileBuffer = new char[size];

    // Create a cursor to keep track of the current position in the buffer
    char* cursor = *fileBuffer;

    // Store the header (ranges)
    uint ranges[2] = { ourMesh->indices.size(), ourMesh->ourVertex.size() };
    uint bytes = headerSize;
    memcpy(cursor, ranges, bytes);
    cursor += bytes;

    // Store the indices
    bytes = sizeof(uint) * ourMesh->indices.size();
    memcpy(cursor, ourMesh->indices.data(), bytes);
    cursor += bytes;

    // Store the vertices
    bytes = sizeof(float) * 3 * ourMesh->ourVertex.size() + sizeof(float) * 3 * ourMesh->ourVertex.size() + sizeof(float) * 2 * ourMesh->ourVertex.size();
    memcpy(cursor, ourMesh->ourVertex.data(), bytes);
    cursor += bytes;

    // Return the size of the file buffer
    return size;
}

void CFF_Mesh::Load(const char* fileBuffer, ModuleMesh::Mesh* ourMesh)
{
    char* cursor = const_cast<char*>(fileBuffer);

    // Load the header (ranges)
    uint ranges[2];
    uint bytes = sizeof(ranges);
    memcpy(ranges, cursor, bytes);
    cursor += bytes;

    // Resize indices and vertices
    ourMesh->indices.resize(ranges[0]);
    ourMesh->ourVertex.resize(ranges[1]);

    // Load indices
    bytes = sizeof(uint) * ourMesh->indices.size();
    memcpy(ourMesh->indices.data(), cursor, bytes);
    cursor += bytes;

    // Load vertices
    bytes = sizeof(float) * 3 * ourMesh->ourVertex.size() + sizeof(float) * 3 * ourMesh->ourVertex.size() + sizeof(float) * 2 * ourMesh->ourVertex.size();
    memcpy(ourMesh->ourVertex.data(), cursor, bytes);
    cursor += bytes;
}

bool CFF_Mesh::SaveMeshToFile(const ModuleMesh::Mesh* ourMesh, const std::string& filename) {

    char* fileBuffer;

    uint bufferSize = CFF_Mesh::Save(ourMesh, &fileBuffer);

    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile.is_open()) {

        LOG("Error: Unable to open the file for writing: %s", filename);

        return false;
    }

    // Write the buffer to the file
    outFile.write(fileBuffer, bufferSize);

    // Close the file
    outFile.close();

    // Free the allocated memory for the buffer
    delete[] fileBuffer;

    return true;
}
