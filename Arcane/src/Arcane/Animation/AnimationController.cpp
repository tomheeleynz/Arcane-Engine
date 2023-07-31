#include "AnimationController.h"

namespace Arcane
{
	AnimationController::AnimationController()
	{
	}

	Animation* AnimationController::GetAnimation(std::string animationName)
	{
		return m_Animations[animationName];
	}

	Animation* AnimationController::GetCurrentAnimation()
	{
		if (m_Animations.find(m_CurrentAnimation) == m_Animations.end())
			return nullptr;

		return m_Animations[m_CurrentAnimation];
	}
}