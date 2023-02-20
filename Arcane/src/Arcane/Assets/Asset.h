#pragma once

#include <string>
#include "Arcane/Core/UUID.h"

namespace Arcane
{
	enum class AssetType
	{
		MESH,
		TEXTURE,
		MATERIAL,
		SCRIPT,
		SHADER,
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
	private:
		Core::UUID m_UUID;
		AssetType m_Type;
		std::string m_Name;
	};
}