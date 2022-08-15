#pragma once

namespace Arcane
{
	enum class AssetType
	{
		MESH
	};

	class Asset
	{
	public:
		Asset() {}

		int GetID() { return m_ID; }
		void SetID(int id) { m_ID = id; };

		AssetType GetAssetType() { return m_Type; };
		void SetAssetType(AssetType type) { m_Type = type; };
	private:
		int m_ID = -1;
		AssetType m_Type;
	};
}