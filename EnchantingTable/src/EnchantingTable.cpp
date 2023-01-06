#include <filesystem>
#include <Arcane.h>

#include "EditorLayer.h"
#include "OpenGLTestLayer.h"

class EnchantingTableApplication : public Arcane::Application
{
public:
	EnchantingTableApplication(Arcane::ApplicationSpecifications& specifications) : Arcane::Application(specifications)
	{
		//PushLayer(new OpenGLTestLayer());
		PushLayer(new EditorLayer());
	}
private:

};

int main(int argc, char* argv[])
{
	// Parse Project Directory
	const char* projectFilePath = argv[1];
	std::cout << projectFilePath << std::endl;

	// -- Set API to Vulkan
	Arcane::RendererAPI::SetAPI(Arcane::RendererAPIType::Vulkan);

	// Set Application Specifications
	Arcane::ApplicationSpecifications specs;
	specs.Name = "Arcane Engine";
	specs.WindowWidth = 1600;
	specs.WindowHeight = 1200;
	specs.PushImguiLayer = true;
	specs.ProjectFilePath = projectFilePath;

	// Create Vulkan Application
	EnchantingTableApplication* app = new EnchantingTableApplication(specs);
	app->Run();
	delete app;
}