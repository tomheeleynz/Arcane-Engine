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

			// ReflectModule(vertexBytes, m_VertexShaderModule);
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
		return m_MaterialDescriptorSet;
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

			m_DescriptorSets.resize(sets.size());
			m_ShaderSets.resize(sets.size());

			for (int i = 0; i < sets.size(); i++) {
				SpvReflectDescriptorSet& reflectSet = *(sets[i]);

				// Custom type for platform agnostic to use
				ShaderSet& shaderSet = m_ShaderSets[i];
				shaderSet.SetNumber = reflectSet.set;
				shaderSet.Bindings.resize(reflectSet.binding_count);

				// Create Descriptor Shaders for material 
				DescriptorSetSpecs setSpecs;
				setSpecs.SetNumber = reflectSet.set;
				std::vector<DescriptorLayoutSpecs> layoutSpecs(reflectSet.binding_count, DescriptorLayoutSpecs{});

				for (int j = 0; j < reflectSet.binding_count; j++) {
					SpvReflectDescriptorBinding& reflectBinding = *(reflectSet.bindings[j]);	
					shaderSet.Bindings[j].Binding = reflectBinding.binding;
					shaderSet.Bindings[j].Size = reflectBinding.block.size;

					DescriptorLayoutSpecs& layoutSpec = layoutSpecs[j];
					layoutSpec.Binding = reflectBinding.binding;
					layoutSpec.Name = reflectBinding.name;

					if (reflectBinding.descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
						shaderSet.Bindings[j].Type = ShaderBindingType::SAMPLER;
						layoutSpec.Type = DescriptorType::SAMPLER;
					}
					else if (reflectBinding.descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
						shaderSet.Bindings[j].Type = ShaderBindingType::UNIFORM;
						layoutSpec.Type = DescriptorType::UNIFORM_BUFFER;
					}

					layoutSpec.DescriptorCount = 1;

					for (uint32_t i_dim = 0; i_dim < reflectBinding.array.dims_count; ++i_dim) {
						layoutSpec.DescriptorCount *= reflectBinding.array.dims[i_dim];
					}

					if (module.shader_stage & VK_SHADER_STAGE_VERTEX_BIT) {
						layoutSpec.Location = DescriptorLocation::VERTEX;
					}

					if (module.shader_stage & VK_SHADER_STAGE_FRAGMENT_BIT) {
						layoutSpec.Location = DescriptorLocation::FRAGMENT;
					}

					// Getting member variables
					// if the member count is 0, just get the name of the block
					// if its more than 0, have to recursively find all the members of each block

					if (reflectBinding.block.member_count != 0) {
						m_ShaderSets[i].Bindings[j].Members.resize(reflectBinding.block.member_count);
						
						for (int k = 0; k < reflectBinding.block.member_count; k++) {
							// Setting up my internal structure
							ShaderMember& member = m_ShaderSets[i].Bindings[j].Members[k];
							member.Name = reflectBinding.block.members[k].name;

							// Getting the binding variable
							SpvReflectBlockVariable& variable = reflectBinding.block.members[k];
							
							// Get the members of the above member variable recursively
							FindShaderMembers(variable, member);
						}
					}
					else {
						m_ShaderSets[i].Bindings[j].Members.resize(1);
						m_ShaderSets[i].Bindings[j].Members[0].Name = reflectBinding.name;
					}
				}

				m_DescriptorSets[i] = DescriptorSet::Create(setSpecs, layoutSpecs);

				if (setSpecs.SetNumber == 2)
					m_MaterialDescriptorSet = m_DescriptorSets[i];
			}
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

	void VulkanShader::FindShaderMembers(SpvReflectBlockVariable& variable, ShaderMember& member)
	{
		if (variable.member_count == 0) {
			if (variable.type_description->op == SpvOpTypeVector) {
				if (variable.type_description->traits.numeric.vector.component_count == 3)
				{
					member.type = ShaderMemberType::Vec3;
				}
				else if (variable.type_description->traits.numeric.vector.component_count == 2)
				{
					member.type = ShaderMemberType::Vec2;
				}
				else if (variable.type_description->traits.numeric.vector.component_count == 4) 
				{
					member.type = ShaderMemberType::Vec4;
					member.Offset = variable.offset;
					member.Size = variable.size;
				}
			}
			else if (variable.type_description->op == SpvOpTypeMatrix)
			{
				if (variable.type_description->traits.numeric.matrix.column_count == 4) {
					member.type = ShaderMemberType::Mat4;
				}
				else if (variable.type_description->traits.numeric.matrix.column_count == 3) {
					member.type = ShaderMemberType::Mat3;
				}
			}
			return;
		}
		else {
			member.Members.resize(variable.member_count);

			for (int i = 0; i < variable.member_count; i++) {
				ShaderMember& newMember = member.Members[i];
				SpvReflectBlockVariable newBlockVariable = variable.members[i];

				newMember.Name = newBlockVariable.name;
				FindShaderMembers(newBlockVariable, newMember);
			}
		}
	}
}