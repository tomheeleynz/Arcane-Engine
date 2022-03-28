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
		SceneDeserializer(std::string filepath);

		Scene* Deserialize();
	private:
		std::string m_Filepath;
	};
}