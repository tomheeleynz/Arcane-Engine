#pragma once

#include <filesystem>
#include <fstream>

#include "Arcane/Core/Application.h"

namespace Arcane
{
	class AssetPack
	{
	public:
		AssetPack(std::filesystem::path path);

		void Serialize();
		void Deserialize();
	private:
		std::filesystem::path m_Path;
	};
}