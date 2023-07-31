#pragma once
#include <Arcane.h>
#include <imgui.h>
#include <string>

#include "MaterialViewerPanel.h"

class FileBrowserPanel
{
public:
	FileBrowserPanel();

	void OnUpdate();

	void SetMaterialViewPanel(MaterialViewerPanel* panel) { m_MaterialViewPanel = panel; };
private:
	std::string GetIconType(std::string extension);

	// Creation Functions
	void CreateMaterial(std::string name);
	void CreateScript(std::string name);
	void CreateFolder(std::string name);
	void CreateScene(std::string name);
	void CreateUnlitShader(std::string name);
	void CreateStandardShader(std::string name);
	void CreateAnimation(std::string name);
	void CreateAnimationController(std::string name);

	// Import Asset Function
	void ImportAsset(std::string fileLocation);


private:
	Arcane::FileWatcher* m_Watcher;
	std::map<std::string, ImTextureID> m_Icons;

	std::filesystem::path m_AssetPath;

	MaterialViewerPanel* m_MaterialViewPanel = nullptr;
};