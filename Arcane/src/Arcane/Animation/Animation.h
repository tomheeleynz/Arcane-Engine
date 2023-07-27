#pragma once

#include <vector>
#include <string>
#include <map>
#include "Arcane/Assets/Asset.h"

namespace Arcane
{
	enum class KeyFrameType
	{
		TWO_DIMENSIONAL,
		THREE_DIMENSIONAL
	};

	/////////////////////////////////////////////////
	//// Base Animation Classes
	/////////////////////////////////////////////////
	class KeyFrame
	{
	public:
		KeyFrame() {}
		KeyFrame(KeyFrameType type);

		void SetType(KeyFrameType type) { m_Type = type; }
		KeyFrameType GetType() { return m_Type; }
	protected:
		KeyFrameType m_Type;
	};

	class Animation : public Asset
	{
	public:
		Animation();

		KeyFrame* GetKeyFrame(int index) { return m_KeyFrames[index]; }
		std::map<int, KeyFrame*> GetKeyFrames() { return m_KeyFrames; }

		void ResetKeyFrameCount() { m_CurrentKeyframe = 0; }
		
		void AddKeyFrame(int index, KeyFrame* keyFrame) { m_KeyFrames[index] = keyFrame; }
		KeyFrame* GetCurrentKeyFrame() { return m_KeyFrames[m_CurrentKeyframe]; }
		void SetNextKeyFrame();

		int CurrentFrameCount = 0;
	private:
		std::string m_Name;
		std::map<int, KeyFrame*> m_KeyFrames;
		int m_CurrentKeyframe = 0;
	};

	/////////////////////////////////////////////////
	//// 2D KeyFrame
	/////////////////////////////////////////////////
	class KeyFrame2D : public KeyFrame
	{
	public:
		KeyFrame2D();

		void SetKeyFrameLength(float keyFrameLength) { m_KeyFrameLength = keyFrameLength; }
		float GetKeyFrameLength() { return m_KeyFrameLength; }

		void SetImageIndexX(float imageIndex) { m_ImageIndexX = imageIndex; }
		float GetImageIndexX() { return m_ImageIndexX; }

		void SetImageIndexY(float imageIndex) { m_ImageIndexY = imageIndex; }
		float GetImageIndexY() { return m_ImageIndexY; }
	private:
		float m_KeyFrameLength;
		float m_ImageIndexX;
		float m_ImageIndexY;
	};
}