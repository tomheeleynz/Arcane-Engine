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
		if (m_Material == nullptr)
		{
			jsonObject["name"] = savePath.stem().string();
		}
		else {
			jsonObject["name"] = m_Material->GetName();

			if (m_Material->GetShader() != nullptr) {
				jsonObject["shader"] = m_Material->GetShader()->GetID();

				ShaderSet& set = m_Material->GetMaterialSet();
				jsonObject["setNumber"] = set.SetNumber;

				nlohmann::json bindingArray = nlohmann::json::array();
				for (int i = 0; i < set.Bindings.size(); i++) {
					nlohmann::json bindingObject = nlohmann::json::object();
					ShaderBinding& binding = set.Bindings[i];

					bindingObject["binding"] = binding.Binding;
					bindingObject["size"] = binding.Size;

					if (binding.Type == ShaderBindingType::SAMPLER) {
						bindingObject["type"] = "SAMPLER";
					}
					else if (binding.Type == ShaderBindingType::UNIFORM) {
						bindingObject["type"] = "UNIFORM";
					}

					nlohmann::json memberArray = nlohmann::json::array();
					// Get The Members
					for (int j = 0; j < binding.Members.size(); j++) {
						ShaderMember& member = binding.Members[j];

						if (member.Members.size() != 0) {
							GetMemberArray(member, memberArray, binding.Binding);
						}
						else {
							nlohmann::json memberObject = nlohmann::json::object();

							memberObject["name"] = member.Name;
							memberObject["size"] = member.Size;
							memberObject["offset"] = member.Offset;
							memberObject["type"] = member.type;

							if (binding.Type == ShaderBindingType::SAMPLER) {
								memberObject["texture"] = m_Material->GetTexture(binding.Binding)->GetID();
							}

							if (member.type == ShaderMemberType::Vec3) {
								glm::vec3 value = m_Material->GetVec3(binding.Binding, member.Offset);
								memberObject["value"] = { value.x, value.y, value.z };
							}

							memberArray += memberObject;
						}
					}
					bindingObject["members"] = memberArray;
					bindingArray += bindingObject;
				}

				jsonObject["bindings"] = bindingArray;
			}
		}
		// Save json objet to file
		std::ofstream ofs;
		ofs.open(savePath.string().c_str(), std::ofstream::out | std::ofstream::trunc);
		ofs << std::setw(4) << jsonObject << std::endl;
		ofs.close();
	}

	void MaterialSerializer::GetMemberArray(ShaderMember& member, nlohmann::json memberArray, uint32_t binding)
	{
		if (member.Members.size() != 0) {
			GetMemberArray(member, memberArray, binding);
		}
		else {
			nlohmann::json memberObject = nlohmann::json::object();
			memberObject["name"] = member.Name;
			memberObject["size"] = member.Size;
			memberObject["offset"] = member.Offset;
			memberObject["type"] = member.type;

			if (member.type == ShaderMemberType::Vec3) {
				glm::vec3 value = m_Material->GetVec3(binding, member.Offset);
				memberObject["value"] = {value.x, value.y, value.z};
			}

			memberArray += memberObject;
		}

	}
}