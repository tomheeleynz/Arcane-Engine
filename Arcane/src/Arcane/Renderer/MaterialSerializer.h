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

		void GetMemberArray(ShaderMember& member, nlohmann::json memberArray, uint32_t binding);
	private:
		Material* m_Material;
	
	};
}