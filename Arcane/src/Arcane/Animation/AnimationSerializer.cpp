#include <fstream>
#include <nlohmann/json.hpp>

#include "AnimationSerializer.h"

namespace Arcane
{
	AnimationSerializer::AnimationSerializer(Animation* animation)
	{
		m_Animation = animation;
	}

	void AnimationSerializer::Serialize(std::filesystem::path path)
	{
		nlohmann::json jsonObject;

		// Set name of animation
		jsonObject["name"] = m_Animation->GetName();

		nlohmann::json keyframeArray = nlohmann::json::array();

		for (auto const& [key, val] : m_Animation->GetKeyFrames())
		{
			nlohmann::json keyFrameObject = nlohmann::json::array();
			KeyFrameType type = val->GetType();

			keyFrameObject["type"] = type == KeyFrameType::TWO_DIMENSIONAL ? "2D" : "3D";
			
			if (type == KeyFrameType::TWO_DIMENSIONAL) {
				KeyFrame2D* keyFrame2D = static_cast<KeyFrame2D*>(val);
				
				keyFrameObject["ImageIndexX"] = keyFrame2D->GetImageIndexX();
				keyFrameObject["ImageIndexY"] = keyFrame2D->GetImageIndexY();
				keyFrameObject["KeyframeLength"] = keyFrame2D->GetKeyFrameLength();
			}

			keyFrameObject["order"] = key;
			keyframeArray.push_back(keyFrameObject);
		}

		jsonObject["Keyframes"] = keyframeArray;
		
		std::ofstream o(path);
		o << std::setw(4) << jsonObject << std::endl;
	}

	void AnimationSerializer::Deserialize(std::filesystem::path path)
	{
		nlohmann::json jsonObject;
		std::ifstream i(path);
		i >> jsonObject;

		m_Animation->SetName(jsonObject["name"]);

		for (auto& element : jsonObject["Keyframes"])
		{
			KeyFrameType type = element["type"] == "3D" ? KeyFrameType::THREE_DIMENSIONAL : KeyFrameType::TWO_DIMENSIONAL;

			if (type == KeyFrameType::TWO_DIMENSIONAL)
			{
				KeyFrame2D* newKeyFrame = new KeyFrame2D();
				newKeyFrame->SetImageIndexX(element["ImageIndexX"]);
				newKeyFrame->SetImageIndexY(element["ImageIndexY"]);
				newKeyFrame->SetKeyFrameLength(element["KeyframeLength"]);
				m_Animation->AddKeyFrame(element["order"], newKeyFrame);
			}
		}
	}
}