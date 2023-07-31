#include "AnimationControllerPanel.h"

AnimationControllerPanel::AnimationControllerPanel()
{
    m_Delegate = GraphEditorDelegate();
    m_Fit = GraphEditor::Fit_None;
}

void AnimationControllerPanel::OnImGuiRender()
{
	ImGui::Begin("Animation Controller");
	{
        if (ImGui::CollapsingHeader("Graph Editor"))
        {
            GraphEditor::EditOptions(m_Options);
        }

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
	}
	ImGui::End();
}

void AnimationControllerPanel::SetAnimationController(Arcane::AnimationController* controller)
{
	m_AnimationController = controller;
}
