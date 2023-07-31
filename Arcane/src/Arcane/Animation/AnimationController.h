#pragma once

#include <map>
#include <string>

#include "Animation.h"

namespace Arcane
{
	struct AnimationLink
	{
		Animation* from;
		Animation* to;
	};

	class AnimationController : public Asset
	{
	public:
		AnimationController();

		Animation* GetCurrentAnimation();
		Animation* GetAnimation(std::string animationName);

		void SetCurrentAnimation(std::string name) { m_CurrentAnimation = name; }
		void AddAnimation(std::string name, Animation* animation) { m_Animations[name] = animation; };

		std::map<std::string, Animation*> GetAnimations() { return m_Animations; }

		std::vector<AnimationLink> GetLinks() { return m_AnimationLinks; }
		void AddLink(AnimationLink& link) { m_AnimationLinks.push_back(link); }
	private:
		std::vector<AnimationLink> m_AnimationLinks;
		std::map<std::string, Animation*> m_Animations;
		std::string m_CurrentAnimation;
	};
}