#include "FileBrowser.h"
#include "Arcane.h"
#include "PanelStructs.h"



FileBrowserPanel::FileBrowserPanel()
{
	m_Watcher = new Arcane::FileWatcher(Arcane::Application::Get().GetProject()->GetWorkingPath().string(), std::chrono::milliseconds(5000));

	///////////////////////////////////////////
	//// Icons
	///////////////////////////////////////////
	
	// -- Folder Icon
	m_Icons["Folder"] = Arcane::UI::AddTexture(Arcane::Texture::Create("./src/EditorAssets/Icons/folder_icon.png"));

	// -- Image Icon
	m_Icons["Image"] = Arcane::UI::AddTexture(Arcane::Texture::Create("./src/EditorAssets/Icons/image_icon.png"));

	// -- Mesh Icon
	m_Icons["Mesh"] = Arcane::UI::AddTexture(Arcane::Texture::Create("./src/EditorAssets/Icons/mesh_icon.png"));

	// -- Script Icon
	m_Icons["Script"] = Arcane::UI::AddTexture(Arcane::Texture::Create("./src/EditorAssets/Icons/script_icon.png"));

	// -- Shader Icon
	m_Icons["Shader"] = Arcane::UI::AddTexture(Arcane::Texture::Create("./src/EditorAssets/Icons/shader_icon.png"));

	// -- Material Icon
	m_Icons["Material"] = Arcane::UI::AddTexture(Arcane::Texture::Create("./src/EditorAssets/Icons/material_icon.png"));

	// -- Scene Icon
	m_Icons["Scene"] = Arcane::UI::AddTexture(Arcane::Texture::Create("./src/EditorAssets/Icons/scene_icon.png"));
}

void FileBrowserPanel::OnUpdate()
{
	m_Watcher->Update();
	
	ImGui::Begin("File Browser");
	{
		if (ImGui::Button("Back")) {
			m_Watcher->GoToParentDirectory();
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		int id = 0;
		for (auto& path : m_Watcher->GetPaths())
		{
			if (!path.second.isDirectory) {
				std::string iconType = GetIconType(path.second.relativePath.extension().string());

				Arcane::UI::Image(m_Icons[iconType], {thumbnailSize, thumbnailSize});
				
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					// Create Asset Info
					AssetInfo newInfo;
					newInfo.id = path.second.assetID;
					newInfo.name = path.second.name;
					ImGui::SetDragDropPayload("CURRENT_SELECTED_ASSET", &newInfo, sizeof(AssetInfo));
					ImGui::EndDragDropSource();
				}

				ImGui::Text(path.second.relativePath.stem().string().c_str());
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));

				ImGui::PushID(id);
				
				if (Arcane::UI::ImageButton(m_Icons["Folder"], {thumbnailSize, thumbnailSize})) {
					m_Watcher->SetDirectory(path.second.relativePath);
				}

				ImGui::PopID();
				id++;
				ImGui::PopStyleColor(2);
				ImGui::Text(path.second.relativePath.stem().string().c_str());
			}
			ImGui::NextColumn();
		}
	}
	ImGui::Columns(1);

	// Popup to create new files 
	if (ImGui::Button("Create Asset"))
		ImGui::OpenPopup("CreateAsset");

	if (ImGui::BeginPopup("CreateAsset"))
	{
		if (ImGui::MenuItem("New Material"))
		{
			std::string filename = Arcane::FileDialog::SaveFile();

			if (!filename.empty()) {
				Arcane::MaterialSerializer serializer(nullptr);
				serializer.Serialize(filename);
				// Save asset to database
				Arcane::Application::Get().GetAssetDatabase().GenerateAsset(std::filesystem::path(filename), true);
			}

			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::End();
}

std::string FileBrowserPanel::GetIconType(std::string extension)
{
	if (extension == ".jpg" || extension == ".jpeg" || extension == ".png") {
		return "Image"; 
	}

	if (extension == ".arcanemat") {
		return "Material";
	}

	if (extension == ".obj" || extension == ".fbx" || extension == ".arcaneproj") {
		return "Mesh";
	}

	if (extension == ".cs") {
		return "Script";
	}

	if (extension == ".arcaneshader") {
		return "Shader";
	}

	if (extension == ".arcanescene") {
		return "Scene";
	}

	return std::string();
}