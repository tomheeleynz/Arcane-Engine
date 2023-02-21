#pragma once

#include <string>
#include <filesystem>

#include "Arcane/Core/UUID.h"

namespace Arcane
{
	enum class AssetType
	{
		MESH = 0,
		TEXTURE,
		SCRIPT,
		SHADER,
		MATERIAL,
		SCENE
	};

	class Asset
	{
	public:
		Asset() {}

		uint64_t GetID() { return (uint64_t)m_UUID; }
		void SetID(Core::UUID id) { m_UUID = id; };

		AssetType GetAssetType() { return m_Type; };
		void SetAssetType(AssetType type) { m_Type = type; };

		std::string GetName() { return m_Name; }
		void SetName(std::string name) { m_Name = name; }

		std::filesystem::path GetPath() { return m_Path; }
		void SetPath(std::filesystem::path path) { m_Path = path; }
	private:
		Core::UUID m_UUID;
		AssetType m_Type;
		std::string m_Name;
		std::filesystem::path m_Path;
	};
}