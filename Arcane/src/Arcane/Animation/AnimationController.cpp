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
		return m_Animations[m_CurrentAnimation];
	}
}