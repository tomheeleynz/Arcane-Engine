#include "AnimationPanel.h"
#include "PanelStructs.h"

AnimationPanel::AnimationPanel()
{
	m_AnimationSequencer = AnimationSequencer();
	m_AnimationSequencer.m_FrameMin = 0;
	m_AnimationSequencer.m_FrameMax = 100;
}

void AnimationPanel::OnImGuiRender()
{
	ImGui::Begin("Animation");
	{
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

			if (payload != nullptr) {
				// Get Asset Id
				AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
				Arcane::Asset* animationAsset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

				if (animationAsset != nullptr && animationAsset->GetAssetType() == Arcane::AssetType::ANIMATION) {
					Arcane::Animation* animation = static_cast<Arcane::Animation*>(animationAsset);
					m_Animation = animation;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (m_Animation == nullptr) {
			if (ImGui::Button("Create Animation"))
				CreateAnimation();
		}
		else {
			for (const auto& [key, val] : m_Animation->GetKeyFrames())
			{
				if (val->GetType() == Arcane::KeyFrameType::TWO_DIMENSIONAL)
				{
					Arcane::KeyFrame2D* keyFrame2D = static_cast<Arcane::KeyFrame2D*>(val);
					AnimationSequencer::KeyFrameItem newItem;
					if (key != 0) {
						AnimationSequencer::KeyFrameItem prevItem = m_AnimationSequencer.keyFrameItems[key - 1];
						newItem.frameStart = prevItem.frameEnd;
						newItem.frameEnd = newItem.frameStart + keyFrame2D->GetKeyFrameLength();
					}
					else {
						newItem.frameStart = 0;
						newItem.frameEnd = keyFrame2D->GetKeyFrameLength();
					}

					m_AnimationSequencer.keyFrameItems.push_back(newItem);
				}
			}

			int currentFrame = 0;
			int selectedEntry = -1;
			bool expanded = true;
			ImSequencer::Sequencer(&m_AnimationSequencer, &currentFrame, &expanded, &selectedEntry, 0, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
		}
	}
	ImGui::End();
}

void AnimationPanel::CreateAnimation()
{
	std::string filename = Arcane::FileDialog::SaveFile();

	if (!filename.empty()) {
		m_Animation = new Arcane::Animation();
		Arcane::AnimationSerializer serializer(m_Animation);
		std::filesystem::path animationPath(filename);

		std::string name = animationPath.stem().string();
		m_Animation->SetName(name);

		serializer.Serialize(animationPath);

	}
}