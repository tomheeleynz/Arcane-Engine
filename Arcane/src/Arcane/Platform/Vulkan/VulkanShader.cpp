#include "Arcane/Core/Application.h"
#include "VulkanShader.h"
#include "VulkanContext.h"

namespace Arcane {
	static std::vector<char> readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			printf("File Not Open\n");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	VulkanShader::VulkanShader(std::string vertexShader, std::string fragmentShader)
	{
		// Get Vulkan Context to be able to get the devices
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		VulkanContext* _context = static_cast<VulkanContext*>(window.GetContext());

		// -- Get Logical Device
		VkDevice& logicalDevice = _context->GetDevice().GetLogicalDevice();

		// Create Vertex Shader
		{
			auto vertexShaderFile = readFile(vertexShader);
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = vertexShaderFile.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(vertexShaderFile.data());

			if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &m_VertexShaderModule) != VK_SUCCESS) {
				printf("Vertex shader not created\n");
			}
			else {
				printf("Vertex shader created\n");
			}

			m_VertexByteCode = vertexShaderFile;
		}

		// Create Fragment Shader
		{
			auto fragmentShaderFile = readFile(fragmentShader);
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = fragmentShaderFile.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(fragmentShaderFile.data());


			if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &m_FragShaderModule) != VK_SUCCESS) {
				printf("Fragment shader not created\n");
			}
			else {
				printf("Fragment shader created\n");
			}

			m_FragmentByteCode = fragmentShaderFile;
		}


	}

	void VulkanShader::Reflect() 
	{
		// Reflect Fragment Module
		{
			SpvReflectShaderModule module;

			SpvReflectResult result = spvReflectCreateShaderModule(
				m_FragmentByteCode.size(),
				m_FragmentByteCode.data(),
				&module
			);

			if (result != SPV_REFLECT_RESULT_SUCCESS) {
				printf("Failed to Create Reflect Module\n");
			}

			uint32_t count = 0;
			result = spvReflectEnumerateDescriptorSets(&module, &count, nullptr);
			
			if (result != SPV_REFLECT_RESULT_SUCCESS) {
				printf("Failed to enum descriptor sets\n");
			}

			std::vector<SpvReflectDescriptorSet*> sets(count);
			result = spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
			
			if (result != SPV_REFLECT_RESULT_SUCCESS) {
				printf("Failed to load reflect descriptor sets\n");
			}

			// Get the material set, which will be set 2 binding 0
			for (int i = 0; i < sets.size(); i++) 
			{
				SpvReflectDescriptorSet* reflectSet = sets[i];
			
				
				// Get into the bindings,
				// Material will be binding 0
				for (int j = 0; j < reflectSet->binding_count; j++) {
					SpvReflectDescriptorBinding* binding = reflectSet->bindings[j];
					
					if (binding->binding == 0) {
						// Get stuff in the material block
						SpvReflectTypeDescription* description = binding->type_description;
						
						for (int k = 0; k < description->member_count; k++) {
							SpvReflectTypeDescription memberDesc = description->members[k];
							ShaderVariable newVariable;
							newVariable.Name = memberDesc.struct_member_name;
							
							if (memberDesc.traits.numeric.vector.component_count == 3) {
								newVariable.Type = ShaderVariableType::Vector3;
							}

							m_ShaderVariables.push_back(newVariable);
						}
					}
				}
			}


		}
	}
}