#include <glad/glad.h>
#include "OpenGLTestLayer.h"

OpenGLTestLayer::OpenGLTestLayer()
{
}

void OpenGLTestLayer::OnAttach()
{
	Arcane::Shader* testShader = Arcane::Shader::Create(
		".\\src\\EditorAssets\\Shaders\\MeshVert.spv",
		".\\src\\EditorAssets\\Shaders\\MeshTextured.spv"
	);
}

void OpenGLTestLayer::OnDetach()
{
}

void OpenGLTestLayer::OnUpdate(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void OpenGLTestLayer::OnImGuiRender()
{
}
