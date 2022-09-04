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

private:
	Arcane::FileWatcher* m_Watcher;
	std::map<std::string, Arcane::Texture*> m_Icons;
};