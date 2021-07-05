#include <filesystem>
#include <Arcane.h>

#include "EditorLayer.h"

class EnchantingTableApplication : public Application
{
public:
	EnchantingTableApplication(ApplicationSpecifications& specifications) : Application(specifications)
	{
		PushLayer(new EditorLayer());
	}


private:

};

int main()
{
	// -- Set API to Vulkan
	RendererAPI::SetAPI(RendererAPIType::Vulkan);

	// Set Application Specifications
	ApplicationSpecifications specs;
	specs.Name = "Arcane Engine";
	specs.WindowWidth = 1600;
	specs.WindowHeight = 1200;

	// Create Vulkan Application
	EnchantingTableApplication* app = new EnchantingTableApplication(specs);
	app->Run();
	delete app;
}