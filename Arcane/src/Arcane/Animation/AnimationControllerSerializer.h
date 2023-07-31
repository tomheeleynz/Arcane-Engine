#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>

#include "AnimationController.h"

namespace Arcane
{
	class AnimationControllerSerializer
	{
	public:
		AnimationControllerSerializer(AnimationController* controller);

		void Serialize(std::filesystem::path path);
		void Deserialize(std::filesystem::path path);
	private:
		AnimationController* m_AnimationController;
	};
}