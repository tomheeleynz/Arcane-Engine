#include <nlohmann/json.hpp> 
#include "FileWatcher.h"
namespace Arcane
{
	FileWatcher::FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay)
	{
		m_Directory = path_to_watch;
		//for (auto& file : std::filesystem::directory_iterator(path_to_watch)) {

		//	m_Paths[file.path().string()] = std::filesystem::last_write_time(file);
		//}
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

			if (file.path().extension() != ".arcmeta") {
				if (!contains(file.path().string())) 
				{
			
					FileInfo newFileInfo;
					newFileInfo.name = file.path().stem().string();
				
					if (file.is_regular_file())
						newFileInfo.assetID = GetAssetID(file.path());
					else
						newFileInfo.isDirectory = true;

					m_Paths[file.path().string()] = newFileInfo;
				}
			}
		}
	}

	int FileWatcher::GetAssetID(std::filesystem::path metaPath)
	{
		std::string metaFileName = metaPath.stem().string() + ".arcmeta";
		std::filesystem::path metaFile = metaPath.parent_path() / metaFileName;
		
		std::ifstream i(metaFile.string());

		nlohmann::json metaJson;
		i >> metaJson;

		return metaJson["id"];
	}
}