#include "SubMesh.h"

namespace Arcane
{
	SubMesh::SubMesh(aiMesh* submesh)
	{
		// Get Vertices
		for (int i = 0; i < submesh->mNumVertices; i++) {
			SubMeshVertex newVertex;
			
			// Get Position
			newVertex.position = { 
				submesh->mVertices[i].x,
				submesh->mVertices[i].y,
				submesh->mVertices[i].z
			};

			// Get Normal
			newVertex.normal = {
				submesh->mNormals[i].x,
				submesh->mNormals[i].y,
				submesh->mNormals[i].z
			};

			newVertex.texture = {
				submesh->mTextureCoords[0][i].x,
				submesh->mTextureCoords[0][i].y
			};

			m_Vertices.push_back(newVertex);
		}

		for (int i = 0; i < submesh->mNumFaces; i++) {
			aiFace face = submesh->mFaces[i];

			for (int j = 0; j < face.mNumIndices; j++) {
				m_Indices.push_back(face.mIndices[j]);
			}
		}
	}
}