#pragma once

#include <nlohmann/json.hpp>

#include "Scene.h"

namespace Arcane
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* scene);

		void Serialize(std::string filepath);
	private:
		Scene* m_Scene = nullptr;
	};
}