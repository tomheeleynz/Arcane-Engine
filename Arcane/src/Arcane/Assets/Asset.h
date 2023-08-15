#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <map>

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
		ANIMATION,
		ANIMATION_CONTROLLER,
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

		// Virtual Methods
		virtual void PackAsset(std::ofstream& o) {}
		virtual std::pair<uint64_t, Asset*> UnpackAsset(std::ofstream& o) { return std::pair<uint64_t, Asset*>(); };
	private: 
		Core::UUID m_UUID;
		AssetType m_Type;
		std::string m_Name;
		std::filesystem::path m_Path;
	};
}