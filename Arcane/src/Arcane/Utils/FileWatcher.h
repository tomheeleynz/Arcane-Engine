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
	int assetID = -1;
	bool isDirectory = false;
};

namespace Arcane
{
	class FileWatcher
	{
	public:
		FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay);
		void Update();


		std::unordered_map<std::string, FileInfo> GetPaths() { return m_Paths; }
	private:
		int GetAssetID(std::filesystem::path metaPath);
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