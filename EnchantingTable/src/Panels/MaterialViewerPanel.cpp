#include "MaterialViewerPanel.h"
#include "PanelStructs.h"

MaterialViewerPanel::MaterialViewerPanel()
{

}

void MaterialViewerPanel::OnUpdate()
{
	using namespace Arcane;
	if (m_Material == nullptr) return;
	
	ImGui::Begin("Material Viewer");

	ImGui::Text("Shader");

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

		if (payload != nullptr) {
			AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
			Asset* asset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

			if (asset != nullptr && asset->GetAssetType() == AssetType::SHADER)
			{
				Shader* shader = static_cast<Shader*>(asset);
				m_Material->SetShader(shader);
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (m_Material->GetShader() != nullptr) {
		std::vector<Arcane::ShaderVariable> materialVariables = m_Material->GetMaterialVariables();
		for (auto& variable : materialVariables)
		{
			// Image Sampler
			if (variable.Type == Arcane::ShaderVariableType::Sampler)
			{
				Arcane::Texture* texture = m_Material->GetTexture(variable.binding);
				Arcane::UI::Image(texture);

				// Add drag and drop
				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

					if (payload != nullptr) {
						AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
						Asset* asset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

						if (asset != nullptr && asset->GetAssetType() == AssetType::TEXTURE)
						{
							Texture* texture = static_cast<Texture*>(asset);
							m_Material->WriteTexture(variable.binding, texture);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}
		}
	}
	
	if (ImGui::Button("Save Material"))
	{
		std::string filename = Arcane::FileDialog::SaveFile();

		if (!filename.empty()) {
			Arcane::MaterialSerializer serializer(m_Material);
			serializer.Serialize(filename);
		}
	}

	ImGui::End();
}

void MaterialViewerPanel::SetMaterial(Arcane::Material* material)
{
	m_Material = material;
}
