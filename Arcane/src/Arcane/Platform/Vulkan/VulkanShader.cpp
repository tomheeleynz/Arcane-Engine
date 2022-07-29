#include "Arcane/Core/Application.h"
#include "Arcane/Renderer/DescriptorSet.h"
#include "VulkanShader.h"
#include "VulkanContext.h"

namespace Arcane {

	struct DescriptorSetLayoutData
	{
		uint32_t SetNumber;
		VkDescriptorSetLayoutCreateInfo CreateInfo;
		std::vector<VkDescriptorSetLayoutBinding> Bindings;
		VkDescriptorSetLayout Layout;
	};

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

		// Reflect Shader to get data
		Reflect();
	}

	DescriptorSet* VulkanShader::GetMaterialDescriptor()
	{
		return m_MaterialSet;
	}

	void VulkanShader::Reflect() 
	{
		// Reflect Fragment Module
		ReflectModule(m_FragmentByteCode, m_FragShaderModule);
	}

	void VulkanShader::ReflectModule(std::vector<char> byteCode, VkShaderModule module)
	{
		// Get Vulkan Context to be able to get the devices
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		VulkanContext* _context = static_cast<VulkanContext*>(window.GetContext());
		uint32_t imageCount = _context->GetSwapChain().GetSwapChainImagesSize();

		// -- Get Logical Device
		VkDevice& logicalDevice = _context->GetDevice().GetLogicalDevice();

		// Reflect Fragment Module
		std::vector<DescriptorSetLayoutData> setLayouts;
		{
			SpvReflectShaderModule module;

			SpvReflectResult result = spvReflectCreateShaderModule(
				byteCode.size(),
				byteCode.data(),
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

			uint32_t offset = 0;
			uint32_t size = 0;
			for (int i = 0; i < sets.size(); i++) {
				SpvReflectDescriptorSet* reflectSet = sets[i];
				// DescriptorSetLayoutData newData;

				if (reflectSet->set == 2)
				{
					DescriptorSetSpecs newSetSpecs;
					newSetSpecs.SetNumber = reflectSet->set;

					std::vector<DescriptorLayoutSpecs> bindings;
					bindings.resize(reflectSet->binding_count);
					for (int j = 0; j < reflectSet->binding_count; j++) {
						// Get the reflected binding
						const SpvReflectDescriptorBinding& reflBinding = *(reflectSet->bindings[j]);
						ShaderVariable newVariable;
						
						for (int k = 0; k < reflBinding.type_description->member_count; k++)
						{
							// This gets the member variable
							SpvReflectTypeDescription& memberDesc = reflBinding.type_description->members[k];

							if (memberDesc.traits.numeric.vector.component_count == 3)
							{
								// This is a vector 3
								newVariable.Type = ShaderVariableType::Vec3;
								newVariable.offset = offset;
								newVariable.size = sizeof(float) * 3;
								newVariable.Name = memberDesc.struct_member_name;
								
								m_MaterialVariables.push_back(newVariable);
								
								size += sizeof(float) * 3;
								offset += 3;
							}
						}

						// Get the actual material variables
						if (reflBinding.binding == 0)
						{
							// Create my struct for a binding
							DescriptorLayoutSpecs& binding = bindings[j];
							
							// Binding number
							binding.Binding = reflBinding.binding;
							
							// location
							binding.Location = DescriptorLocation::FRAGMENT;
							
							// descriptor count
							binding.DescriptorCount = 1;
							
							// material
							binding.Name = "Material";
							
							// Uniform Buffer
							binding.Type = DescriptorType::UNIFORM_BUFFER;
						}
					}

					m_MaterialSet = DescriptorSet::Create(newSetSpecs, bindings);
				}
			}
			m_MaterialSize = size;
		}
	}
}