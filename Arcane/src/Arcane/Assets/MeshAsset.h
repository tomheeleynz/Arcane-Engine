#pragma once
#include <filesystem>
#include "Asset.h"
#include "Arcane/Renderer/Mesh.h"

namespace Arcane
{
	class MeshAsset : public Asset
	{
	public:
		MeshAsset(std::filesystem::path meshPath);


		Mesh* GetMesh() { return m_Mesh; }
	private:
		Mesh* m_Mesh;
	};
}