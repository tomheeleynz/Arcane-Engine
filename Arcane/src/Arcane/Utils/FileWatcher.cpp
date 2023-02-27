#include <nlohmann/json.hpp> 
#include "FileWatcher.h"
namespace Arcane
{
	FileWatcher::FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay)
	{
		m_Directory = path_to_watch;
	}

	void FileWatcher::Update()
	{
		auto it = m_Paths.begin();
		while (it != m_Paths.end())
		{
			if (!std::filesystem::exists(it->first)) {
				it = m_Paths.erase(it);
			}
			else {
				it++;
			}
		}

		for (auto& file : std::filesystem::directory_iterator(m_Directory)) {
			auto current_file_last_write_time = std::filesystem::last_write_time(file);

			if (file.path().extension() != ".arcmeta" && file.path().stem() != "venv" && file.path().extension() != ".arcaneproj" && file.path().extension() != ".pyi") {
				if (!contains(file.path().string())) 
				{
					FileInfo newFileInfo;
					newFileInfo.name = file.path().stem().string();
				
					if (file.is_regular_file())
						newFileInfo.assetID = GetAssetID(file.path());
					else
						newFileInfo.isDirectory = true;

					newFileInfo.relativePath = file.path();
					m_Paths[file.path().string()] = newFileInfo;
				}
			}
		}
	}

	uint64_t FileWatcher::GetAssetID(std::filesystem::path metaPath)
	{
		std::string metaFileName = metaPath.stem().string() + ".arcmeta";
		std::filesystem::path metaFile = metaPath.parent_path() / metaFileName;
		
		std::ifstream i(metaFile.string());

		nlohmann::json metaJson;
		i >> metaJson;

		return metaJson["id"];
	}

	void FileWatcher::SetDirectory(std::filesystem::path newPath)
	{
		m_Directory = newPath.string();
		m_Paths.clear();
	}

	void FileWatcher::GoToParentDirectory()
	{
		std::filesystem::path currentPath = std::filesystem::path(m_Directory);
		std::filesystem::path parentPath = currentPath.parent_path();
		m_Directory = parentPath.string();
		m_Paths.clear();
	}
}