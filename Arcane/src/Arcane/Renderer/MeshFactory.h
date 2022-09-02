#pragma once

#include "Arcane/Renderer/Mesh.h"

namespace Arcane
{
	class MeshFactory
	{
	public:
		static MeshFactory* GetInstance();

		static Mesh* CreatePlane();
	private:
		MeshFactory() {};
		static MeshFactory* s_Instance;
	private:
		Mesh* CreatePlaneImpl();
	};
}