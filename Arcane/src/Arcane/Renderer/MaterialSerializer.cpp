#include "MaterialSerializer.h"

namespace Arcane
{
	MaterialSerializer::MaterialSerializer(Material* material)
	{
		m_Material = material;
	}

	void MaterialSerializer::Serialize(std::string path)
	{
		// Setting up json object
		std::filesystem::path savePath(path);
		nlohmann::json jsonObject;

		// Set Name of scene
		jsonObject["name"] = "DefaultMaterial";
		jsonObject["shader"] = m_Material->GetShader()->GetID();

		// variable array
		nlohmann::json variableArray = nlohmann::json::array();
		
		// Iterate through shader variables and serialize variables 
		std::vector<ShaderVariable> shaderVariables = m_Material->GetMaterialVariables();
		for (ShaderVariable shaderVariable : shaderVariables) {
			nlohmann::json variableObject = nlohmann::json::object();

			if (shaderVariable.Type == ShaderVariableType::Sampler) {
				Texture* materialTexture = m_Material->GetTexture(shaderVariable.binding);
				variableObject["name"] = shaderVariable.Name;
				variableObject["assetID"] = materialTexture->GetID();
			}

			variableArray += variableObject;
		}

		jsonObject["variables"] = variableArray;

		// Save json objet to file
		std::ofstream o(savePath.string());
		o << std::setw(4) << jsonObject << std::endl;
	}
}