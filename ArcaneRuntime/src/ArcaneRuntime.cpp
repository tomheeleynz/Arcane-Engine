#include <Arcane.h>

#include "RuntimeLayer.h"

class ArcaneRuntimeApplication : public Arcane::Application
{
public:
	ArcaneRuntimeApplication(Arcane::ApplicationSpecifications& specifications) : Arcane::Application(specifications)
	{
		PushLayer(new RuntimeLayer());
	}

private:

};

int main(int argc, char* argv[])
{
	Arcane::ApplicationSpecifications specs;
	specs.Name = "Arcane Engine";
	specs.WindowWidth = 1200;
	specs.WindowHeight = 800;

	specs.PushImguiLayer = false;
	specs.RuntimeLayer = true;
	specs.RuntimePath = "C:\\Projects\\Arcane-Engine\\ArcaneRuntime\\res\\test.arcanepack";

	ArcaneRuntimeApplication* app = new ArcaneRuntimeApplication(specs);
	app->Run();
	delete app;
}