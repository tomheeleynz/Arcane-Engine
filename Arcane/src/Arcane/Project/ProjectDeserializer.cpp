#include "ProjectDeserializer.h"

namespace Arcane
{
	ProjectDeserializer::ProjectDeserializer(std::filesystem::path path)
	{
		m_Path = path;
	}

	Project* ProjectDeserializer::Deserialize()
	{
		ProjectSpecs newSpecs;

		// Read Json File
		nlohmann::json jsonObject;
		std::ifstream i(m_Path.string());
		i >> jsonObject;

		// Create project

		// -- Name
		std::string name = jsonObject["name"];
		newSpecs.name = name;

		// -- Working Directory
		std::string workingDirPath = jsonObject["workingdir"];
		newSpecs.workingDir = std::filesystem::path(workingDirPath);

		// -- Whether to start up the project in 2D or 3D mode
		std::string dimensionType = jsonObject["dimensiontype"];

		if (dimensionType == "2D")
			newSpecs.dimensionType = DimensionType::TwoD;
		else
			newSpecs.dimensionType = DimensionType::ThreeD;


		Project* newProject = new Project(newSpecs);
		return newProject;
	}
}