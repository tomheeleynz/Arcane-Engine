#include <filesystem>
#include <Arcane.h>

#include "EditorLayer.h"

class EnchantingTableApplication : public Arcane::Application
{
public:
	EnchantingTableApplication(Arcane::ApplicationSpecifications& specifications) : Arcane::Application(specifications)
	{
		PushLayer(new EditorLayer());
	}
private:

};

int main()
{
	// -- Set API to Vulkan
	Arcane::RendererAPI::SetAPI(Arcane::RendererAPIType::Vulkan);

	// Set Application Specifications
	Arcane::ApplicationSpecifications specs;
	specs.Name = "Arcane Engine";
	specs.WindowWidth = 1600;
	specs.WindowHeight = 1200;
	specs.PushImguiLayer = true;

	// Create Vulkan Application
	EnchantingTableApplication* app = new EnchantingTableApplication(specs);
	app->Run();
	delete app;
}