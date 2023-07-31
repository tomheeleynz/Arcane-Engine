#include "AnimationControllerPanel.h"
#include "PanelStructs.h"


AnimationControllerPanel::AnimationControllerPanel()
{
    m_Delegate = GraphEditorDelegate();
    m_Fit = GraphEditor::Fit_None;
    m_AnimationController = nullptr;
}

void AnimationControllerPanel::OnImGuiRender()
{
	ImGui::Begin("Animation Controller");
	{
        if (m_AnimationController != nullptr) {
            if (ImGui::Button("Fit all nodes"))
            {
                m_Fit = GraphEditor::Fit_AllNodes;
            }
            ImGui::SameLine();
            if (ImGui::Button("Fit selected nodes"))
            {
                m_Fit = GraphEditor::Fit_SelectedNodes;
            }
            ImGui::SameLine();
            if (ImGui::Button("Apply"))
            {
                UpdateAnimationController();
            }

            if (m_Delegate.GetNodeCount() == 1)
            {
                for (const auto& [key, val] : m_AnimationController->GetAnimations())
                {
                    int indexOfNewNode = m_Delegate.GetNodeCount();
                    GraphEditorDelegate::Node newNode;
                    newNode.name = "Test";
                    newNode.templateIndex = 1;
                    newNode.x = 300.0f;
                    newNode.y = 0.0f;

                    m_Delegate.mNodes.push_back(newNode);
                }
            }

            GraphEditor::Show(m_Delegate, m_Options, m_ViewState, true, &m_Fit);

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

                if (payload != nullptr) {
                    // Get Asset Id
                    AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
                    Arcane::Asset* animationAsset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

                    if (animationAsset != nullptr && animationAsset->GetAssetType() == Arcane::AssetType::ANIMATION) {
                        Arcane::Animation* animation = static_cast<Arcane::Animation*>(animationAsset);
                       
                        int indexOfNewNode = m_Delegate.GetNodeCount();
                        GraphEditorDelegate::Node newNode;
                        newNode.name = "Test";
                        newNode.templateIndex = 1;
                        newNode.x = 300.0f;
                        newNode.y = 0.0f;

                        m_Delegate.mNodes.push_back(newNode);
                        m_AnimationController->AddAnimation(animation->GetName(), animation);
                    }
                }
                ImGui::EndDragDropTarget();
            }

        }
        else {
            ImGui::Text("Drag and Drop Animation Controller to Begin");

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

                if (payload != nullptr) {
                    AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
                    Arcane::Asset* asset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

                    if (asset != nullptr && asset->GetAssetType() == Arcane::AssetType::ANIMATION_CONTROLLER)
                    {
                        Arcane::AnimationController* controller = static_cast<Arcane::AnimationController*>(asset);
                        m_AnimationController = controller;
                    }
                }
                ImGui::EndDragDropTarget();
            }
        }
	}
	ImGui::End();
}

void AnimationControllerPanel::SetAnimationController(Arcane::AnimationController* controller)
{
	m_AnimationController = controller;
}

void AnimationControllerPanel::UpdateAnimationController()
{
    Arcane::AnimationControllerSerializer serializer(m_AnimationController);
    serializer.Serialize(m_AnimationController->GetPath());
}
