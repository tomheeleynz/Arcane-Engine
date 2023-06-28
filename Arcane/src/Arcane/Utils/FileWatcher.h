#pragma once

#include <string>
#include <chrono>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <functional>
#include <fstream>

enum class FileStatus
{
	Created, 
	Deleted,
	Modified
};

struct FileInfo
{
	std::string name;
	uint64_t assetID = 0;
	bool isDirectory = false;
	std::filesystem::path relativePath;
};

namespace Arcane
{
	class FileWatcher
	{
	public:
		FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay);
		
		void SetDirectory(std::filesystem::path newPath);
		void Update();
		void GoToParentDirectory();

		std::filesystem::path GetDirectory() { return std::filesystem::path(m_Directory); }

		std::unordered_map<std::string, FileInfo> GetPaths() { return m_Paths; }
	private:
		uint64_t GetAssetID(std::filesystem::path metaPath);
	private:
		std::chrono::duration<int, std::milli> m_Delay;
		std::unordered_map<std::string, FileInfo> m_Paths;
		std::string m_Directory;

		bool contains(const std::string & key) {
			auto el = m_Paths.find(key);
			return el != m_Paths.end();
		}
	};
}