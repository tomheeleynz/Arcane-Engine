#pragma once

namespace Arcane {
	class Layer
	{
	public:
		Layer() {};

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnImGuiRender() {}
	private:

	};
}