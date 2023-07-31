#include "AnimationControllerSerializer.h"
#include "Arcane/Core/Application.h"

namespace Arcane
{
	AnimationControllerSerializer::AnimationControllerSerializer(AnimationController* controller)
	{
		m_AnimationController = controller;
	}

	void AnimationControllerSerializer::Serialize(std::filesystem::path path)
	{
		nlohmann::json jsonObject;
		jsonObject["name"] = path.stem().string();

		if (m_AnimationController->GetCurrentAnimation() != nullptr)
			jsonObject["current"] = m_AnimationController->GetCurrentAnimation()->GetID();

		nlohmann::json animationArray = nlohmann::json::array();
		for (auto const& [key, val] : m_AnimationController->GetAnimations())
		{
			nlohmann::json newAnimation = nlohmann::json::object();
			newAnimation["name"] = key;
			newAnimation["animation"] = val->GetID();
			animationArray.push_back(newAnimation);
		}

		jsonObject["Animations"] = animationArray;
		std::ofstream o(path);
		o << std::setw(4) << jsonObject << std::endl;
	}

	void AnimationControllerSerializer::Deserialize(std::filesystem::path path)
	{
		AssetDatabase& database = Application::Get().GetAssetDatabase();

		nlohmann::json jsonObject;
		std::ifstream i(path);
		i >> jsonObject;

		m_AnimationController->SetName(jsonObject["name"]);
		for (auto& element : jsonObject["Animations"]) {
			Animation* animation = static_cast<Animation*>(database.GetAsset(element["animation"]));
			m_AnimationController->AddAnimation(element["name"], animation);
		}

		if (jsonObject.contains("current"))
		{
			for (auto const& [key, val] : m_AnimationController->GetAnimations())
			{
				if (jsonObject["current"] == val->GetID())
					m_AnimationController->SetCurrentAnimation(key);
			}
		}
	}
}