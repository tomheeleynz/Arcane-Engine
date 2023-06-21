#pragma once

#include <string>
#include <filesystem>

namespace Arcane
{
	enum class DimensionType 
	{
		TwoD,
		ThreeD
	};

	struct ProjectSpecs 
	{
		std::filesystem::path workingDir;
		std::string name;
		DimensionType dimensionType;
	};

	class Project
	{
	public:
		Project(ProjectSpecs& specs);

		std::filesystem::path GetWorkingPath() { return m_Specs.workingDir; }
		DimensionType GetDimensionType() { return m_Specs.dimensionType; }
	private:
		ProjectSpecs m_Specs;
	};
}