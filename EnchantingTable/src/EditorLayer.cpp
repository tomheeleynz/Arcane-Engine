#include <glm/glm.hpp>
#include <vector>
#include <filesystem>

#include "EditorLayer.h"

struct TestVertex
{
	glm::vec3 position;
	glm::vec3 color;
};


EditorLayer::EditorLayer()
{

}

void EditorLayer::OnAttach()
{
	// Test Shader
	Arcane::Shader* shader = Arcane::Shader::Create(".\\src\\Assets\\Shaders\\vert.spv", ".\\src\\Assets\\Shaders\\frag.spv");
	Arcane::RenderPass* renderPass = Arcane::RenderPass::Create();

	// Test Vertex Descriptor
	Arcane::VertexDescriptor* vertexDescriptor = Arcane::VertexDescriptor::Create({
		Arcane::VertexType::float3,
		Arcane::VertexType::float3
	});

	// Test Pipeline
	Arcane::PipelineSpecification spec;
	spec.descriptor = vertexDescriptor;
	spec.renderPass = renderPass;
	spec.shader = shader;
	spec.uniformBuffer = Arcane::UniformBuffer::Create();

	Arcane::Pipeline* pipeline = Arcane::Pipeline::Create(spec);

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
	Arcane::VertexBuffer* vertexBuffer = Arcane::VertexBuffer::Create(vertices.data(), sizeof(TestVertex) * vertices.size());
	Arcane::IndexBuffer* indexBuffer = Arcane::IndexBuffer::Create(indices.data(), indices.size());
	vertexBuffer->AddIndexBuffer(indexBuffer);

	// Begin a Render pass
	Arcane::Renderer::BeginRenderPass(renderPass);

	// Render Test Triangle
	Arcane::Renderer::RenderQuad(vertexBuffer, pipeline);

	// End a pass
	Arcane::Renderer::EndRenderPass(renderPass);
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate(float deltaTime)
{
	
}

void EditorLayer::OnImGuiRender()
{

}
