#include "MaterialViewerPanel.h"

MaterialViewerPanel::MaterialViewerPanel()
{

}

void MaterialViewerPanel::OnUpdate()
{
	using namespace Arcane;
	if (m_Material == nullptr) return;
	
	ImGui::Begin("Material Viewer");
	
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
					int assetID = *static_cast<int*>(payload->Data);
					Asset* asset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetID);

					if (asset != nullptr && asset->GetAssetType() == AssetType::TEXTURE)
					{
						TextureAsset* textureAsset = static_cast<TextureAsset*>(asset);
						textureAsset->LoadAsset();
						m_Material->WriteTexture(variable.binding, textureAsset->GetTexture());
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
	}
	
	ImGui::End();
}

void MaterialViewerPanel::SetMaterial(Arcane::Material* material)
{
	m_Material = material;
}
