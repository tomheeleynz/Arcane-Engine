#include <shaderc/shaderc.hpp>

#include "Arcane/Core/Application.h"
#include "Arcane/Renderer/DescriptorSet.h"
#include "VulkanShader.h"
#include "VulkanContext.h"
#include "VulkanSet.h"

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
		m_ReadFromSPV = true;

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

			ReflectModule(vertexBytes, m_VertexShaderModule);
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

		std::sort(m_ShaderSets.begin(), m_ShaderSets.end(), [](ShaderSet& a, ShaderSet& b)->bool {
			return a.SetNumber < b.SetNumber;
		});

		// Fill in missing sets if need be
		FillMissingSets();

		// Create Descriptor Sets
		GenerateDescriptorSets();
	}

	VulkanShader::VulkanShader(uint32_t* vertexByteCode, uint32_t vertexBytesSize, uint32_t* fragmentByteCode, uint32_t fragmentBytesSize)
	{
		// Get Vulkan Context to be able to get the devices
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		VulkanContext* _context = static_cast<VulkanContext*>(window.GetContext());

		// -- Get Logical Device
		VkDevice& logicalDevice = _context->GetDevice().GetLogicalDevice();

		// Create Vertex Shader
		{
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = vertexBytesSize * 4;
			createInfo.pCode = vertexByteCode;

			if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &m_VertexShaderModule) != VK_SUCCESS) {
				printf("Vertex shader not created\n");
			}
			else {
				printf("Vertex shader created\n");
			}
		}

		// Create Fragment Shader
		{
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = fragmentBytesSize * 4;
			createInfo.pCode = fragmentByteCode;

			if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &m_FragShaderModule) != VK_SUCCESS) {
				printf("Fragment shader not created\n");
			}
			else {
				printf("Fragment shader created\n");
			}
		}
	}

	DescriptorSet* VulkanShader::GetMaterialDescriptor()
	{
		return m_MaterialDescriptorSet;
	}

	void VulkanShader::Reflect() 
	{
	}

	void VulkanShader::PackAsset(std::ofstream& o)
	{
		if (m_ReadFromSPV)
		{
			uint32_t vertexBytesCount = m_VertexByteCode.size();
			uint32_t fragmentBytesCount = m_FragmentByteCode.size();

			o.write((const char*)&vertexBytesCount, sizeof(uint32_t));
			o.write((const char*)m_VertexByteCode.data(), vertexBytesCount * sizeof(char));
			o.write((const char*)&fragmentBytesCount, sizeof(uint32_t));
			o.write((const char*)m_FragmentByteCode.data(), fragmentBytesCount * sizeof(char));
		}
		else {
			ShaderProgramSource sources = ParseShader(GetPath().string());

			// Get Vertex Bytes
			std::vector<uint32_t> vertexBytes = CompileShader("vertexShader", shaderc_glsl_vertex_shader, sources.vertexShader);
			uint32_t vertexBytesSize = vertexBytes.size();

			// Get Fragment Bytes
			std::vector<uint32_t> fragmentBytes = CompileShader("fragmentShader", shaderc_glsl_fragment_shader, sources.fragmentShader);
			uint32_t fragmentBytesSize = fragmentBytes.size();

			// Write both of them to the file
			int type = (int)GetAssetType();
			uint64_t id = GetID();

			// Calculate total length of shader block
			uint32_t totalLength = (sizeof(uint32_t) * 2) + (fragmentBytes.size() * sizeof(uint32_t)) + (vertexBytes.size() * sizeof(uint32_t));
			o.write((const char*)&type, sizeof(int));
			o.write((const char*)&id, sizeof(uint64_t));
			o.write((const char*)&totalLength, sizeof(uint32_t));
			o.write((const char*)&vertexBytesSize, sizeof(uint32_t));
			o.write((const char*)vertexBytes.data(), vertexBytesSize * sizeof(uint32_t));
			o.write((const char*)&fragmentBytesSize, sizeof(uint32_t));
			o.write((const char*)fragmentBytes.data(), fragmentBytesSize * sizeof(uint32_t));
		}
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

			for (int i = 0; i < sets.size(); i++) {
				SpvReflectDescriptorSet& reflectSet = *(sets[i]);
				ShaderSet shaderSet;

				if (CheckIfSetExists(reflectSet.set)) {
					shaderSet = GetExistingSet(reflectSet.set);
				} else {
					// Custom type for platform agnostic to use
					ShaderSet newSet;
					newSet.SetNumber = reflectSet.set;
					newSet.Bindings.resize(reflectSet.binding_count);
					shaderSet = newSet;
				}

				for (int j = 0; j < reflectSet.binding_count; j++) {
					SpvReflectDescriptorBinding& reflectBinding = *(reflectSet.bindings[j]);	
					shaderSet.Bindings[j].Binding = reflectBinding.binding;
					shaderSet.Bindings[j].Size = reflectBinding.block.size;

					if (reflectBinding.descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
						shaderSet.Bindings[j].Type = ShaderBindingType::SAMPLER;
					}
					else if (reflectBinding.descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
						shaderSet.Bindings[j].Type = ShaderBindingType::UNIFORM;
					}

					if (module.shader_stage & VK_SHADER_STAGE_VERTEX_BIT) {
						shaderSet.Bindings[j].Location = ShaderVariableLocation::VERTEX;
					}

					if (module.shader_stage & VK_SHADER_STAGE_FRAGMENT_BIT) {
						shaderSet.Bindings[j].Location = ShaderVariableLocation::FRAGMENT;
					}

					shaderSet.Bindings[j].DescriptorCount = 1;

					for (uint32_t i_dim = 0; i_dim < reflectBinding.array.dims_count; ++i_dim) {
						shaderSet.Bindings[j].DescriptorCount *= reflectBinding.array.dims[i_dim];
					}

					// Getting member variables
					// if the member count is 0, just get the name of the block
					// if its more than 0, have to recursively find all the members of each block
					if (reflectBinding.block.member_count != 0) {
						shaderSet.Bindings[j].Members.resize(reflectBinding.block.member_count);
						
						for (int k = 0; k < reflectBinding.block.member_count; k++) {
							// Setting up my internal structure
							ShaderMember& member = shaderSet.Bindings[j].Members[k];
							member.Name = reflectBinding.block.members[k].name;

							// Getting the binding variable
							SpvReflectBlockVariable& variable = reflectBinding.block.members[k];
							
							// Get the members of the above member variable recursively
							FindShaderMembers(variable, member);
						}
					}
					else {
						shaderSet.Bindings[j].Members.resize(1);
						shaderSet.Bindings[j].Members[0].Name = reflectBinding.name;
					}
				}

				m_ShaderSets.push_back(shaderSet);
			}
		}
	}

	void VulkanShader::GenerateDescriptorSets()
	{
		m_DescriptorSets.resize(m_ShaderSets.size());
		
		for (int i = 0; i < m_ShaderSets.size(); i++) {
			ShaderSet& shaderSet = m_ShaderSets[i];

			// Create Set Specs
			DescriptorSetSpecs setSpecs;
			setSpecs.SetNumber = shaderSet.SetNumber;

			// Create Layout Specs
			std::vector<DescriptorLayoutSpecs> layoutSpecs(shaderSet.Bindings.size(), DescriptorLayoutSpecs{});
			for (int j = 0; j < shaderSet.Bindings.size(); j++) {
				DescriptorLayoutSpecs& layoutSpec = layoutSpecs[j];
				ShaderBinding& binding = shaderSet.Bindings[j];

				// Create Layout Specs
				layoutSpec.Binding = binding.Binding;

				if (binding.Type == ShaderBindingType::SAMPLER) {
					layoutSpec.Type = DescriptorType::SAMPLER;
				}

				if (binding.Type == ShaderBindingType::UNIFORM) {
					layoutSpec.Type = DescriptorType::UNIFORM_BUFFER;
				}

				if (binding.Location == ShaderVariableLocation::VERTEX) {
					layoutSpec.Location = DescriptorLocation::VERTEX;
				}

				if (binding.Location == ShaderVariableLocation::FRAGMENT) {
					layoutSpec.Location = DescriptorLocation::FRAGMENT;
				}

				layoutSpec.DescriptorCount = binding.DescriptorCount;
			}

			m_DescriptorSets[i] = DescriptorSet::Create(setSpecs, layoutSpecs);

			if (setSpecs.SetNumber == 2)
				m_MaterialDescriptorSet = m_DescriptorSets[i];
		}

		std::sort(m_DescriptorSets.begin(), m_DescriptorSets.end(), [](DescriptorSet* a, DescriptorSet* b)->bool {
			VulkanSet* vA = static_cast<VulkanSet*>(a);
			VulkanSet* vB = static_cast<VulkanSet*>(b);
			return vA->GetSetNumber() < vB->GetSetNumber();
		});
	}

	void VulkanShader::FillMissingSets()
	{
		for (int i = 1; i < m_ShaderSets.size() - 1; i++) {
			int current = m_ShaderSets[i].SetNumber;
			int previous = m_ShaderSets[i - 1].SetNumber;

			 if (current != previous + 1) {
				 int missing = previous + 1;

				 ShaderSet newSet;
				 newSet.SetNumber = missing;
				 m_ShaderSets.push_back(newSet);

			}
		}

		std::sort(m_ShaderSets.begin(), m_ShaderSets.end(), [](ShaderSet& a, ShaderSet& b)->bool {
			return a.SetNumber < b.SetNumber;
		});


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

	bool VulkanShader::CheckIfSetExists(uint32_t setNumber)
	{
		for (int i = 0; i < m_ShaderSets.size(); i++) {
			ShaderSet& set = m_ShaderSets[i];
			if (set.SetNumber == setNumber) return true;
		}
		return false;
	}

	ShaderSet& VulkanShader::GetExistingSet(uint32_t setNumber) {
		for (int i = 0; i < m_ShaderSets.size(); i++) {
			if (m_ShaderSets[i].SetNumber == setNumber) {
				return m_ShaderSets[i];
			}
		}
	}

	void VulkanShader::FindShaderMembers(SpvReflectBlockVariable& variable, ShaderMember& member)
	{
		if (variable.member_count == 0) {
			if (variable.type_description->op == SpvOpTypeVector) {
				if (variable.type_description->traits.numeric.vector.component_count == 3)
				{
					member.type = ShaderMemberType::Vec3;
					member.Offset = variable.offset;
					member.Size = variable.size;
				}
				else if (variable.type_description->traits.numeric.vector.component_count == 2)
				{
					member.type = ShaderMemberType::Vec2;
					member.Offset = variable.offset;
					member.Size = variable.size;
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