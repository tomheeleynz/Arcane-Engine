#pragma once

#include "Animation.h"

namespace Arcane
{
	class AnimationSerializer
	{
	public:
		AnimationSerializer(Animation* animation);

		void Serialize(std::filesystem::path path);
		void Deserialize(std::filesystem::path path);
	private:
		Animation* m_Animation;
	};
}