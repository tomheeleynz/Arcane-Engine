#include <shaderc/shaderc.hpp>

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

	VulkanShader::VulkanShader(std::string shaderFile)
	{

		// Get GLSL string of both vertex and fragent shader
		ShaderProgramSource sources = ParseShader(shaderFile);

		// Get Vulkan Context to be able to get the devices
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		VulkanContext* _context = static_cast<VulkanContext*>(window.GetContext());

		// -- Get Logical Device
		VkDevice& logicalDevice = _context->GetDevice().GetLogicalDevice();

		// Create Vertex Shader
		{
			std::vector<uint32_t> vertexBytes = CompileShader("vertexShader", shaderc_glsl_vertex_shader, sources.vertexShader);
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = vertexBytes.size() * 4;
			createInfo.pCode = static_cast<uint32_t*>(vertexBytes.data());

			if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &m_VertexShaderModule) != VK_SUCCESS) {
				printf("Vertex shader not created\n");
			}
			else {
				printf("Vertex shader created\n");
			}
		}

		// Create Fragment Shader
		{
			std::vector<uint32_t> fragmentBytes = CompileShader("fragmentShader", shaderc_glsl_fragment_shader, sources.fragmentShader);
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = fragmentBytes.size() * 4;
			createInfo.pCode = fragmentBytes.data();

			if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &m_FragShaderModule) != VK_SUCCESS) {
				printf("Fragment shader not created\n");
			}
			else {
				printf("Fragment shader created\n");
			}

			ReflectModule(fragmentBytes, m_FragShaderModule);
		}
	}

	DescriptorSet* VulkanShader::GetMaterialDescriptor()
	{
		return m_MaterialSet;
	}

	void VulkanShader::Reflect() 
	{
	}

	void VulkanShader::ReflectModule(std::vector<uint32_t> byteCode, VkShaderModule module)
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
				byteCode.size() * 4,
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
								newVariable.binding = reflBinding.binding;

								m_MaterialVariables.push_back(newVariable);

								size += sizeof(float) * 3;
								offset += 3;
							}
						}

						if (reflBinding.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
							newVariable.Type = ShaderVariableType::Sampler;
							newVariable.binding = reflBinding.binding;
							m_MaterialVariables.push_back(newVariable);
							// Process that material variable
						}

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

						if (reflBinding.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
							binding.Type = DescriptorType::SAMPLER;
						}

						if (reflBinding.descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
							binding.Type = DescriptorType::UNIFORM_BUFFER;
						}
					}

					m_MaterialSet = DescriptorSet::Create(newSetSpecs, bindings);
				}
			}
			m_MaterialSize = size;
		}
	}

	std::vector<uint32_t> VulkanShader::CompileShader(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		
		if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);
		
		std::cout << source << std::endl;
		std::cout << std::endl;
		
		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, kind, source_name.c_str(), options);
		
		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			std::cerr << module.GetErrorMessage();
			return std::vector<uint32_t>();
		}

		return {module.cbegin(), module.cend()};
	}
}