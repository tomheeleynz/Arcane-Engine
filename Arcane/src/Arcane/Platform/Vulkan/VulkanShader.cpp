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
		// Reflect Vertex Module
		{
			SpvReflectShaderModule module;

			SpvReflectResult result = spvReflectCreateShaderModule(
				m_VertexByteCode.size(),
				m_VertexByteCode.data(),
				&module
			);

			if (result != SPV_REFLECT_RESULT_SUCCESS) {
				printf("Vertex Reflect Module not created\n");
			}

			uint32_t bindingCount = 0;
			std::vector<SpvReflectDescriptorBinding*> bindings;
			result = spvReflectEnumerateDescriptorBindings(&module, &bindingCount, NULL);
			
			if (result != SPV_REFLECT_RESULT_SUCCESS) {
				printf("No Binding Descriptors found\n");
			}

			bindings.resize(bindingCount);
			result = spvReflectEnumerateDescriptorBindings(&module, &bindingCount, bindings.data());

			if (result != SPV_REFLECT_RESULT_SUCCESS) {
				printf("Bindings Not Retreived\n");
			}

		}

		// Reflect Fragment Module
		{
			SpvReflectShaderModule module;

			SpvReflectResult result = spvReflectCreateShaderModule(
				m_VertexByteCode.size(),
				m_VertexByteCode.data(),
				&module
			);

			if (result != SPV_REFLECT_RESULT_SUCCESS) {
				printf("Vertex Reflect Module not created\n");
			}

			uint32_t descriptorCount = 0;
			result = spvReflectEnumerateDescriptorSets(&module, &descriptorCount, nullptr);

			if (result != SPV_REFLECT_RESULT_SUCCESS) {
				printf("Cant Reflect Descriptor Sets\n");
			}
			else {
				printf("Descriptor Sets %d\n", descriptorCount);
			}
		}
	}
}