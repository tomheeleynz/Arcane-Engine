#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

struct TestVertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct UniformBufferObject
{
	glm::vec3 color;
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

	m_UniformBuffer = Arcane::UniformBuffer::Create(sizeof(UniformBufferObject));

	// Test Pipeline
	Arcane::PipelineSpecification spec;
	spec.descriptor = m_VertexDescriptor;
	spec.renderPass = m_RenderPass;
	spec.shader = m_Shader;
	spec.uniformBuffer = m_UniformBuffer;

	m_Pipeline = Arcane::Pipeline::Create(spec);

	// Test Vertices
	std::vector<TestVertex> vertices = { 
		{{-0.5f, -0.5f,0.0f}, {1.0f, 0.5f, 0.2f}},
		{{0.5f, -0.5f, 0.0f}, {1.0f, 0.5f, 0.2f}}, 
		{{0.5f, 0.5f,  0.0f}, {1.0f, 0.5f, 0.2f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 0.5f, 0.2f}}
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
	// Test Quad Color
	glm::vec3 quadColor = {1.0f, 1.0f, 1.0f};
	m_UniformBuffer->WriteData(glm::value_ptr(quadColor), sizeof(quadColor));

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
