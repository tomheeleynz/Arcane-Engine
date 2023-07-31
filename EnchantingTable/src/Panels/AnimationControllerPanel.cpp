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
        //if (ImGui::CollapsingHeader("Graph Editor"))
        //{
        //    GraphEditor::EditOptions(m_Options);
        //}

        if (m_AnimationController == nullptr) {
            if (ImGui::Button("Fit all nodes"))
            {
                m_Fit = GraphEditor::Fit_AllNodes;
            }
            ImGui::SameLine();
            if (ImGui::Button("Fit selected nodes"))
            {
                m_Fit = GraphEditor::Fit_SelectedNodes;
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

                        if (m_Delegate.mNodes.size() == 1) {
                            // Create a new node and default attach it to entry node
                            int indexOfNewNode = m_Delegate.GetNodeCount();
                            GraphEditorDelegate::Node newNode;
                            newNode.name = "Test";
                            newNode.templateIndex = 1;
                            newNode.x = 300.0f;
                            newNode.y = 0.0f;

                            // Create a new link
                            GraphEditor::Link newLink;
                            newLink.mInputNodeIndex = 0;
                            newLink.mInputSlotIndex = 0;
                            
                            newLink.mOutputNodeIndex = 1;
                            newLink.mOutputSlotIndex = 0;

                            m_Delegate.mLinks.push_back(newLink);
                            m_Delegate.mNodes.push_back(newNode);
                        }
                        else {
                            // Dont connect to anything
                        }
                    }
                }
                ImGui::EndDragDropTarget();
            }

        }
        else {
            ImGui::Text("Drag and Drop Animation Controller to Begin");
        }
	}
	ImGui::End();
}

void AnimationControllerPanel::SetAnimationController(Arcane::AnimationController* controller)
{
	m_AnimationController = controller;
}
