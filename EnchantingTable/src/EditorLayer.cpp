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
	Shader* shader = Shader::Create(".\\src\\Assets\\Shaders\\vert.spv", ".\\src\\Assets\\Shaders\\frag.spv");
	RenderPass* renderPass = RenderPass::Create();

	// Test Vertex Descriptor
	VertexDescriptor* vertexDescriptor = VertexDescriptor::Create({
		VertexType::float3,
		VertexType::float3
	});

	// Test Pipeline
	PipelineSpecification spec;
	spec.descriptor = vertexDescriptor;
	spec.renderPass = renderPass;
	spec.shader = shader;

	Pipeline* pipeline = Pipeline::Create(spec);

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
	VertexBuffer* vertexBuffer = VertexBuffer::Create(vertices.data(), sizeof(TestVertex) * vertices.size());
	IndexBuffer* indexBuffer = IndexBuffer::Create(indices.data(), indices.size());
	vertexBuffer->AddIndexBuffer(indexBuffer);

	// Begin a Render pass
	Renderer::BeginRenderPass(renderPass);

	// Render Test Triangle
	Renderer::RenderQuad(vertexBuffer, pipeline);

	// End a pass
	Renderer::EndRenderPass(renderPass);

	UniformBuffer* uniformBuffer = UniformBuffer::Create();
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
