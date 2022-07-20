#include "Arcane/Core/Application.h"
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

	void VulkanShader::Reflect() 
	{
		// Reflect Vertex Module
		ReflectModule(m_VertexByteCode, m_VertexShaderModule);

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

			for (int i = 0; i < sets.size(); i++) {
				SpvReflectDescriptorSet* reflectSet = sets[i];
				DescriptorSetLayoutData newData;

				// Resize binding stuff
				newData.Bindings.resize(reflectSet->binding_count);
				for (int j = 0; j < reflectSet->binding_count; j++) {
					// Get the reflected binding
					const SpvReflectDescriptorBinding& reflBinding = *(reflectSet->bindings[j]);

					// Get Descriptor Binding
					VkDescriptorSetLayoutBinding& binding = newData.Bindings[j];

					// Load data into descriptor binding
					binding.binding = reflBinding.binding;
					binding.descriptorType = static_cast<VkDescriptorType>(reflBinding.descriptor_type);
					binding.descriptorCount = 1;
					binding.pImmutableSamplers = nullptr;
					for (uint32_t i_dim = 0; i_dim < reflBinding.array.dims_count; ++i_dim) {
						binding.descriptorCount *= reflBinding.array.dims[i_dim];
					}
					binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				}

				// Load other stuff 
				newData.SetNumber = reflectSet->set;
				newData.CreateInfo = {};
				newData.CreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				newData.CreateInfo.bindingCount = reflectSet->binding_count;
				newData.CreateInfo.pBindings = newData.Bindings.data();
				newData.CreateInfo.flags = 0;

				// Actuall Create the descriptor set layout
				if (vkCreateDescriptorSetLayout(logicalDevice, &newData.CreateInfo, nullptr, &newData.Layout) != VK_SUCCESS) {
					printf("Reflected Shader Descriptor Set layout not created\n");
				}

				setLayouts.push_back(newData);
			}

			VkDescriptorPool descriptorPool = _context->GetPool();
			// Create descriptor sets
			for (int i = 0; i < setLayouts.size(); i++) {
				DescriptorSetLayoutData& setLayout = setLayouts[i];

				for (int j = 0; j < imageCount; j++) {
					VkDescriptorSetAllocateInfo allocInfo = {};
					VkDescriptorSet descriptorSet;
					
					allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
					allocInfo.pNext = nullptr;
					allocInfo.descriptorPool = descriptorPool;
					allocInfo.descriptorSetCount = 1;
					allocInfo.pSetLayouts = &setLayout.Layout;

					if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descriptorSet)) {
						printf("Failed to allocate descriptor set\n");
					}

					m_DescriptorSets.push_back(descriptorSet);
				}
			}
		}
	}
}