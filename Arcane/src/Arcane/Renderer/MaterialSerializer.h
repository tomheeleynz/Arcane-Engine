#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include "Arcane/Renderer/Material.h"

namespace Arcane
{
	class MaterialSerializer
	{
	public:
		MaterialSerializer(Material* material);
		void Serialize(std::string path);
	private:
		Material* m_Material;
	
	};
}