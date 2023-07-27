#pragma once

#include <map>
#include <string>

#include "Animation.h"

namespace Arcane
{
	class AnimationController : public Asset
	{
	public:
		AnimationController();

		Animation* GetCurrentAnimation();
		Animation* GetAnimation(std::string animationName);

		void SetCurrentAnimation(std::string name) { m_CurrentAnimation = name; }
		void AddAnimation(std::string name, Animation* animation) { m_Animations[name] = animation; };
	private:
		std::map<std::string, Animation*> m_Animations;
		std::string m_CurrentAnimation;
	};
}