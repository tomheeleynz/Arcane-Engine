#pragma once

#include <string>
#include <filesystem>

namespace Arcane
{
	struct ProjectSpecs 
	{
		std::filesystem::path workingDir;
		std::string name;
	};

	class Project
	{
	public:
		Project(ProjectSpecs& specs);

		std::filesystem::path GetWorkingPath() { return m_Specs.workingDir; }
	private:
		ProjectSpecs m_Specs;
	};
}