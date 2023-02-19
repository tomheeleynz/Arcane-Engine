#include "MaterialDeserializer.h"

#include "Arcane/Core/Application.h"
#include "Arcane/Assets/AssetDatabase.h"

namespace Arcane
{
	MaterialDeserializer::MaterialDeserializer(std::filesystem::path path)
	{
		m_Path = path;
	}

	Material* MaterialDeserializer::Deserialize()
	{
		// Reading in material json 
		nlohmann::json jsonObject;
		std::ifstream i(m_Path.string());
		i >> jsonObject;

		// Get name of material
		std::string name = jsonObject["name"];
		uint64_t shaderID = jsonObject["shader"];

		// Get the asset database
		Shader* shader = static_cast<Shader*>(Application::Get().GetAssetDatabase().GetAsset(shaderID));
		
		// Creating material
		Material* material = Material::Create(shader);
		
		// Iterate through bindings to get data into buffers
		for (auto& element : jsonObject["bindings"]) {
			uint32_t bindingNum = element["binding"];

			for (auto& member : element["members"]) {
				int type = member["type"];
				int offset = member["offset"];

				if (type == 1) {
					glm::vec3 value = { member["value"][0], member["value"][1] , member["value"][2] };
					material->WriteVec3(bindingNum, offset, value);
				}
				else if (type == 0) {
					uint64_t textureID = member["texture"];
					Texture* texture = static_cast<Texture*>(Application::Get().GetAssetDatabase().GetAsset(textureID));
					material->WriteTexture(bindingNum, texture);
				}
			}
		}

		return material;
	}
}