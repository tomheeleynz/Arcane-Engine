#pragma once

#include <string>
#include <chrono>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <functional>

enum class FileStatus
{
	Created, 
	Deleted,
	Modified
};

namespace Arcane
{
	class FileWatcher
	{
	public:
		FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay);
		void Start(const std::function<void(std::string, FileStatus)>& action);

	private:
		std::chrono::duration<int, std::milli> m_Delay;
		std::unordered_map<std::string, std::filesystem::file_time_type> m_Paths;
		std::string m_Directory;

		bool contains(const std::string & key) {
			auto el = m_Paths.find(key);
			return el != m_Paths.end();
		}
	};
}