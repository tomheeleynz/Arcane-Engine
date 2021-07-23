#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EditorLayer.h"

struct TestVertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};


EditorLayer::EditorLayer()
{

}

void EditorLayer::OnAttach()
{
	// Test Shader
	m_Shader = Arcane::Shader::Create(".\\src\\Assets\\Shaders\\vert.spv", ".\\src\\Assets\\Shaders\\frag.spv");
	m_RenderPass = Arcane::RenderPass::Create();

	// Test Vertex Descriptor
	m_VertexDescriptor = Arcane::VertexDescriptor::Create({
		Arcane::VertexType::float3,
		Arcane::VertexType::float3
	});

	// Test Pipeline
	Arcane::PipelineSpecification spec;
	spec.descriptor = m_VertexDescriptor;
	spec.renderPass = m_RenderPass;
	spec.shader = m_Shader;
	spec.uniformBuffer = Arcane::UniformBuffer::Create(sizeof(UniformBufferObject));

	m_Pipeline = Arcane::Pipeline::Create(spec);

	// Test Vertices
	std::vector<TestVertex> vertices = { 
		{{-0.5f, -0.5f,0.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
	};

	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	// Test Vertex Buffer
	m_VertexBuffer = Arcane::VertexBuffer::Create(vertices.data(), sizeof(TestVertex) * vertices.size());
	Arcane::IndexBuffer* indexBuffer = Arcane::IndexBuffer::Create(indices.data(), indices.size());
	m_VertexBuffer->AddIndexBuffer(indexBuffer);
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate(float deltaTime)
{
	printf("%.2f\n", deltaTime);

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), 1600.0f / 1200.0f, 0.1f, 10.0f);

	// Begin a Render pass
	Arcane::Renderer::BeginRenderPass(m_RenderPass);

	// Render Test Triangle
	Arcane::Renderer::RenderQuad(m_VertexBuffer, m_Pipeline);

	// End a pass
	Arcane::Renderer::EndRenderPass(m_RenderPass);
}

void EditorLayer::OnImGuiRender()
{

}
