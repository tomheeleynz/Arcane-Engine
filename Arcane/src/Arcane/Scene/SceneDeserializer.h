#pragma once

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Scene.h"

namespace Arcane
{
	class SceneDeserializer
	{
	public:
		SceneDeserializer() = default;
		SceneDeserializer(std::string filepath, bool file = true);

		Scene* Deserialize();
		Scene* Deserialize(nlohmann::json jsonObject);
	private:
		std::string m_Filepath;
		std::string m_JSONString;

		bool m_IsFile = true;
	};
}