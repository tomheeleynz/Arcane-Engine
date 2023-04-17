#include <Arcane.h>

class ArcaneRuntime : public Arcane::Application
{
public:
	ArcaneRuntime(Arcane::ApplicationSpecifications& specifications) : Arcane::Application(specifications)
	{
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
	specs.WindowWidth = 1200;
	specs.WindowHeight = 800;
	specs.PushImguiLayer = true;
	specs.ProjectFilePath = projectFilePath;
}