#include "FileWatcher.h"

namespace Arcane
{
	FileWatcher::FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay)
	{
		m_Directory = path_to_watch;
		for (auto& file : std::filesystem::recursive_directory_iterator(path_to_watch)) {
			m_Paths[file.path().string()] = std::filesystem::last_write_time(file);
		}
	}

	void FileWatcher::Start(const std::function<void(std::string, FileStatus)>& action)
	{
		auto it = m_Paths.begin();
		while (it != m_Paths.end())
		{
			if (!std::filesystem::exists(it->first)) {
				action(it->first, FileStatus::Deleted);
				it = m_Paths.erase(it);
			}
			else {
				it++;
			}
		}

		for (auto& file : std::filesystem::recursive_directory_iterator(m_Directory)) {
			auto current_file_last_write_time = std::filesystem::last_write_time(file);

			if (!contains(file.path().string())) 
			{
				m_Paths[file.path().string()] = current_file_last_write_time;
				action(file.path().string(), FileStatus::Created);
			}
			else {
				if (m_Paths[file.path().string()] != current_file_last_write_time) {
					m_Paths[file.path().string()] = current_file_last_write_time;
					action(file.path().string(), FileStatus::Modified);
				}
			}
		}


	}
}