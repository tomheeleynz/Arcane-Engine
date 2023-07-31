#include "FileBrowser.h"
#include "Arcane.h"
#include "PanelStructs.h"

FileBrowserPanel::FileBrowserPanel()
{
	m_Watcher = new Arcane::FileWatcher(Arcane::Application::Get().GetProject()->GetWorkingPath().string(), std::chrono::milliseconds(5000));

	///////////////////////////////////////////
	//// Icons
	///////////////////////////////////////////
	
	std::string dir = std::string(Arcane::Application::Get().GetEditorAssetPath());

	// -- Folder Icon
	m_Icons["Folder"] = Arcane::UI::AddTexture(Arcane::Texture::Create(dir + "/src/EditorAssets/Icons/folder_icon.png"));

	// -- Image Icon
	m_Icons["Image"] = Arcane::UI::AddTexture(Arcane::Texture::Create(dir + "/src/EditorAssets/Icons/image_icon.png"));

	// -- Mesh Icon
	m_Icons["Mesh"] = Arcane::UI::AddTexture(Arcane::Texture::Create(dir + "/src/EditorAssets/Icons/mesh_icon.png"));

	// -- Script Icon
	m_Icons["Script"] = Arcane::UI::AddTexture(Arcane::Texture::Create(dir + "/src/EditorAssets/Icons/script_icon.png"));

	// -- Shader Icon
	m_Icons["Shader"] = Arcane::UI::AddTexture(Arcane::Texture::Create(dir + "/src/EditorAssets/Icons/shader_icon.png"));

	// -- Material Icon
	m_Icons["Material"] = Arcane::UI::AddTexture(Arcane::Texture::Create(dir + "/src/EditorAssets/Icons/material_icon.png"));

	// -- Scene Icon
	m_Icons["Scene"] = Arcane::UI::AddTexture(Arcane::Texture::Create(dir + "/src/EditorAssets/Icons/scene_icon.png"));

	// -- Animation Icon
	m_Icons["Animation"] = Arcane::UI::AddTexture(Arcane::Texture::Create(dir + "/src/EditorAssets/Icons/animation_icon.png"));
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

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.3f));

				ImGui::PushID(id);

				if (Arcane::UI::ImageButton(m_Icons[iconType], { thumbnailSize, thumbnailSize })) {	
					Arcane::AssetType type = Arcane::Application::Get().GetAssetDatabase().GetAsset(path.second.assetID)->GetAssetType();
					
					if (type == Arcane::AssetType::MATERIAL) {
						Arcane::Material* material = static_cast<Arcane::Material*>(Arcane::Application::Get().GetAssetDatabase().GetAsset(path.second.assetID));
						if (m_MaterialViewPanel != nullptr)
							m_MaterialViewPanel->SetMaterial(material);
					}
				}
				
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					// Create Asset Info
					AssetInfo newInfo;
					newInfo.id = path.second.assetID;
					newInfo.name = path.second.name;
					ImGui::SetDragDropPayload("CURRENT_SELECTED_ASSET", &newInfo, sizeof(AssetInfo));
					ImGui::EndDragDropSource();
				}

				ImGui::PopID();
				id++;
				ImGui::PopStyleColor(2);
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

	// Create Booleans
	bool bCreateFolder = false;
	bool bCreateMaterial = false;
	bool bCreateScript = false;
	bool bCreateScene = false;
	bool bCreateUnlitShader = false;
	bool bCreateStandardShader = false;
	bool bCreateAnimation = false;
	bool bCreateAnimationController = false;

	// Import boolean
	bool bImport = false;

	// Create Menu for creating objects
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create Folder")) {
			bCreateFolder = true;
		}

		if (ImGui::MenuItem("Import Asset")) {
			bImport = true;
		}

		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Material")) {
				bCreateMaterial = true;
			}

			if (ImGui::MenuItem("Script")) {
				bCreateScript = true;
			}

			if (ImGui::MenuItem("Scene")) {
				bCreateScene = true;
			}

			if (ImGui::BeginMenu("Shader"))
			{
				if (ImGui::MenuItem("New Unlit Shader")) {
					bCreateUnlitShader = true;
				}

				if (ImGui::MenuItem("New Standard Shader")) {
					bCreateStandardShader = true;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Animation"))
			{
				if (ImGui::MenuItem("Animation")) {
					bCreateAnimation = true;
				}

				if (ImGui::MenuItem("Animation Controller")) {
					bCreateAnimationController = true;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
	
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (bCreateFolder)
		ImGui::OpenPopup("CreateFolderModal");
	
	if (bCreateMaterial)
		ImGui::OpenPopup("MaterialModal");

	if (bCreateScript)
		ImGui::OpenPopup("ScriptModal");

	if (bCreateScene)
		ImGui::OpenPopup("SceneModal");

	if (bImport)
		ImGui::OpenPopup("Import");
	
	if (bCreateUnlitShader)
		ImGui::OpenPopup("Create Unlit Shader");

	if (bCreateStandardShader)
		ImGui::OpenPopup("Create Standard Shader");

	if (bCreateAnimation)
		ImGui::OpenPopup("Create Animation");

	if (bCreateAnimationController)
		ImGui::OpenPopup("Create Animation Controller");

	if (ImGui::BeginPopupModal("CreateFolderModal", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char buf1[64] = ""; 
		ImGui::InputText("Folder Name", buf1, 64);

		if (ImGui::Button("CreateFolder")) {
			CreateFolder(std::string(buf1));
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("MaterialModal", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buf1[64] = "";
		ImGui::InputText("Material Name", buf1, 64);

		if (ImGui::Button("Create Material")) {
			CreateMaterial(std::string(buf1));
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("ScriptModal", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buf1[64] = "";
		ImGui::InputText("Script Name", buf1, 64);

		if (ImGui::Button("Create Script")) {
			CreateScript(std::string(buf1));
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("SceneModal", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buf1[64] = "";
		ImGui::InputText("Scene Name", buf1, 64);

		if (ImGui::Button("Create Scene")) {
			CreateScene(std::string(buf1));
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Import", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buf1[64] = "";
		ImGui::InputText("File Location", buf1, 64);
		
		if (ImGui::Button("Open File")) {
			std::string filename = Arcane::FileDialog::OpenFile();

			if (!filename.empty()) {
				memset(buf1, 0, sizeof(buf1));
				std::strncpy(buf1, filename.c_str(), sizeof(buf1));
			}
		}

		if (ImGui::Button("Import File")) {
			ImportAsset(std::string(buf1));
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Create Unlit Shader", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buf1[64] = "";
		ImGui::InputText("Shader Name", buf1, 64);
		
		if (ImGui::Button("Create")) {
			CreateUnlitShader(std::string(buf1));
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Create Standard Shader", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buf1[64] = "";
		ImGui::InputText("Shader Name", buf1, 64);

		if (ImGui::Button("Create")) {
			CreateStandardShader(std::string(buf1));
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Create Animation", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buf1[64] = "";
		ImGui::InputText("Animation Name", buf1, 64);

		if (ImGui::Button("Create")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Create Animation Controller", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buf1[64] = "";
		ImGui::InputText("Animation Controller Name", buf1, 64);

		if (ImGui::Button("Create")) {
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

	if (extension == ".lua") {
		return "Script";
	}

	if (extension == ".arcaneshader") {
		return "Shader";
	}

	if (extension == ".arcanescene") {
		return "Scene";
	}

	if (extension == ".arcaneanim") {
		return "Animation";
	}

	return std::string();
}

void FileBrowserPanel::CreateMaterial(std::string name)
{
	// Create File
	Arcane::MaterialSerializer serializer(nullptr);
	std::filesystem::path newFilePath = m_Watcher->GetDirectory() / name;
	newFilePath.replace_extension("arcanemat");

	serializer.Serialize(newFilePath.string());

	// Generate Asset
	Arcane::AssetDatabase& database = Arcane::Application::Get().GetAssetDatabase();
	database.GenerateAsset(newFilePath, true);
}

void FileBrowserPanel::CreateScript(std::string name)
{
	// Create File
	std::filesystem::path newFilePath = m_Watcher->GetDirectory() / name;
	newFilePath.replace_extension("lua");
	
	std::ofstream outfile(newFilePath);
	outfile.close();

	// Add Asset to asset database
	Arcane::AssetDatabase& database = Arcane::Application::Get().GetAssetDatabase();
	database.GenerateAsset(newFilePath, true);
}

void FileBrowserPanel::CreateFolder(std::string name)
{
	std::filesystem::path newFolderPath = m_Watcher->GetDirectory() / name;
	std::filesystem::create_directory(newFolderPath);
}

void FileBrowserPanel::CreateScene(std::string name)
{
	// Create File
	std::filesystem::path newFilePath = m_Watcher->GetDirectory() / name;
	newFilePath.replace_extension("arcanescene");
	Arcane::SceneSerializer serializer(nullptr);
	serializer.Serialize(newFilePath.string());
	
	// Add to database
	Arcane::AssetDatabase& database = Arcane::Application::Get().GetAssetDatabase();
	database.GenerateAsset(newFilePath, true);
}

void FileBrowserPanel::CreateUnlitShader(std::string name)
{
	std::string dir = std::string(Arcane::Application::Get().GetEditorAssetPath());

	// Create filepath
	std::filesystem::path newFilePath = m_Watcher->GetDirectory() / name;
	newFilePath.replace_extension("arcaneshader");

	// Write Template to file 
	std::fstream in(dir + "\\src\\EditorAssets\\Templates\\ShaderTemplates\\UnlitShaderTemplate.txt", std::ios::in);
	std::fstream out(newFilePath, std::ios::out);

	std::string ch;
	while(!in.eof()) {
		std::getline(in, ch);
		out << ch << std::endl;
	}

	in.close();
	out.close();

	// Add to asset database
	Arcane::AssetDatabase& database = Arcane::Application::Get().GetAssetDatabase();
	database.GenerateAsset(newFilePath, true);
}

void FileBrowserPanel::CreateStandardShader(std::string name)
{
	std::string dir = std::string(Arcane::Application::Get().GetEditorAssetPath());

	// Create filepath
	std::filesystem::path newFilePath = m_Watcher->GetDirectory() / name;
	newFilePath.replace_extension("arcaneshader");

	// Write Template to file 
	std::fstream in(dir + "\\src\\EditorAssets\\Templates\\ShaderTemplates\\StandardShaderTemplate.txt", std::ios::in);
	std::fstream out(newFilePath, std::ios::out);

	std::string ch;
	while (!in.eof()) {
		std::getline(in, ch);
		out << ch << std::endl;
	}

	in.close();
	out.close();

	// Add to asset database
	Arcane::AssetDatabase& database = Arcane::Application::Get().GetAssetDatabase();
	database.GenerateAsset(newFilePath, true);
}

void FileBrowserPanel::CreateAnimation(std::string name)
{

}

void FileBrowserPanel::CreateAnimationController(std::string name)
{

}

void FileBrowserPanel::ImportAsset(std::string fileLocation)
{
	std::filesystem::path from(fileLocation);
	std::filesystem::path to = m_Watcher->GetDirectory() / from.stem().string();
	to.replace_extension(from.extension());

	std::filesystem::copy_file(from, to);

	Arcane::AssetDatabase& database = Arcane::Application::Get().GetAssetDatabase();
	database.GenerateAsset(to, true);
}
