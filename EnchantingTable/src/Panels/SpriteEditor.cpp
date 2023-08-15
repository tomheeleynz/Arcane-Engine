#include "SpriteEditor.h"
#include "PanelStructs.h"
#include <fstream>

SpriteEditor::SpriteEditor()
{

}

void SpriteEditor::OnImGuiRender()
{
	using namespace Arcane;

	const char* items[] = {"Single", "Multiple"};

	std::map<int, TextureImageAmountType> IndexToType = {
		{0, TextureImageAmountType::SINGLE},
		{1, TextureImageAmountType::MULTIPLE}
	};

	std::map<TextureImageAmountType,int> TypeToIndex = {
		{TextureImageAmountType::SINGLE, 0},
		{TextureImageAmountType::MULTIPLE,1}
	};

	static int item_current_idx = 0;

	ImGui::Begin("Sprite Editor");
	{
		if (m_Texture == nullptr) {
			ImGui::Text("Drag and Drop Texture");
		}
		else {
			TextureSpecs& textureSpecs = m_Texture->GetTextureSpecs();


			item_current_idx = TypeToIndex[textureSpecs.amountType];
			const char* combo_label = items[item_current_idx];

			if (ImGui::BeginCombo("Texture Type", combo_label))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);

					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			textureSpecs.amountType = IndexToType[item_current_idx];

			if (textureSpecs.amountType == TextureImageAmountType::MULTIPLE)
			{
				ImGui::InputFloat("Cell Width", &textureSpecs.cellHeight);
				ImGui::InputFloat("Cell Height", &textureSpecs.cellWidth);

				for (int i = 0; i < textureSpecs.width; i+= textureSpecs.cellWidth)
				{
					float x = i;

					for (int j = 0; j < textureSpecs.height; j+= textureSpecs.cellHeight)
					{
						float y = j;
						
						TextureSpriteRect newRect;
						newRect.x = x;
						newRect.y = y;
						newRect.width = textureSpecs.cellWidth;
						newRect.height = textureSpecs.cellHeight;

						m_TextureSpriteRects.push_back(newRect);
					}
				}
			}

			Arcane::UI::Image(m_Texture, {(float)textureSpecs.width, (float)textureSpecs.width});

			if (ImGui::Button("Apply"))
				ApplyMetaInfo();
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CURRENT_SELECTED_ASSET");

			if (payload != nullptr) {
				AssetInfo assetInfo = *static_cast<AssetInfo*>(payload->Data);
				Asset* asset = Arcane::Application::Get().GetAssetDatabase().GetAsset(assetInfo.id);

				if (asset != nullptr && asset->GetAssetType() == AssetType::TEXTURE)
				{
					Texture* texture = static_cast<Texture*>(asset);
					std::filesystem::path metaPath = asset->GetPath().replace_extension("arcmeta");
					std::ifstream i(metaPath);
					i >> m_TextureMetaFile;
					m_Texture = texture;
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	ImGui::End();
}

void SpriteEditor::ApplyMetaInfo()
{
	m_TextureMetaFile["CellWidth"] = m_Texture->GetTextureSpecs().cellWidth;
	m_TextureMetaFile["CellHeight"] = m_Texture->GetTextureSpecs().cellHeight;
	m_TextureMetaFile["CellCount"] = m_Texture->GetTextureSpecs().cellCount;
	m_TextureMetaFile["Type"] = m_Texture->GetTextureSpecs().amountType == Arcane::TextureImageAmountType::MULTIPLE ? "M" : "S";
	
	std::filesystem::path metaFilePath = m_Texture->GetPath().replace_extension("arcmeta");
	
	std::ofstream o(metaFilePath);
	o << std::setw(4) << m_TextureMetaFile << std::endl;
}
