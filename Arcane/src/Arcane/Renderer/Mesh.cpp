#include <iostream>
#include "Mesh.h"

namespace Arcane
{
	static const uint32_t s_MeshImportFlags = aiProcess_Triangulate | aiProcess_GenBoundingBoxes;

	Mesh::Mesh(std::string filepath)
	{
		Assimp::Importer import;

		const aiScene* scene = import.ReadFile(
			filepath, 
			s_MeshImportFlags
		);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}

        m_BoundingBox.bbMax = scene->mMeshes[0]->mAABB.mMax;
        m_BoundingBox.bbMin = scene->mMeshes[0]->mAABB.mMin;

		ProcessNode(scene->mRootNode, scene);
	}

    Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices)
    {
        m_SubMeshes.push_back(new SubMesh(vertices, indices));
    }

	void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_SubMeshes.push_back(ProcessMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}


	SubMesh* Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;

        // Min Point
        if (mesh->mAABB.mMin.x < m_BoundingBox.bbMin.x)
            m_BoundingBox.bbMin.x = mesh->mAABB.mMin.x;

        if (mesh->mAABB.mMin.y < m_BoundingBox.bbMin.y)
            m_BoundingBox.bbMin.y = mesh->mAABB.mMin.y;

        if (mesh->mAABB.mMin.z < m_BoundingBox.bbMin.z)
            m_BoundingBox.bbMin.z = mesh->mAABB.mMin.z;

        // Max Point
        if (mesh->mAABB.mMax.x > m_BoundingBox.bbMax.x)
            m_BoundingBox.bbMax.x = mesh->mAABB.mMax.x;

        if (mesh->mAABB.mMax.y > m_BoundingBox.bbMax.y)
            m_BoundingBox.bbMax.y = mesh->mAABB.mMax.y;

        if (mesh->mAABB.mMax.z > m_BoundingBox.bbMax.z)
            m_BoundingBox.bbMax.z = mesh->mAABB.mMax.z;
        
        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            MeshVertex vertex;
            glm::vec3 vector;
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }

            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texture = vec;
            }
            else
                vertex.texture = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return new SubMesh(vertices, indices);
	}
}
