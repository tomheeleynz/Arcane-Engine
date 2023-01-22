#pragma once

#include "Arcane/Core/UUID.h"

namespace Arcane
{
	enum class AssetType
	{
		MESH,
		TEXTURE,
		MATERIAL,
		SCRIPT,
		SHADER
	};

	class Asset
	{
	public:
		Asset() {}

		uint64_t GetID() { return (uint64_t)m_UUID; }
		void SetID(Core::UUID id) { m_UUID = id; };

		AssetType GetAssetType() { return m_Type; };
		void SetAssetType(AssetType type) { m_Type = type; };
	private:
		Core::UUID m_UUID;
		AssetType m_Type;
	};
}