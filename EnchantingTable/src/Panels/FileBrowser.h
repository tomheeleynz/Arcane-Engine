#pragma once
#include <Arcane.h>
#include <imgui.h>
#include <string>

class FileBrowserPanel
{
public:
	FileBrowserPanel();

	void OnUpdate();
private:
	std::string GetIconType(std::string extension);

	// Creation Functions
	void CreateMaterial(std::string name);
	void CreateScript(std::string name);
	void CreateFolder(std::string name);
	void CreateScene(std::string name);

	// Import Asset Function
	void ImportAsset(std::string fileLocation);
private:
	Arcane::FileWatcher* m_Watcher;
	std::map<std::string, ImTextureID> m_Icons;

	std::filesystem::path m_AssetPath;
};