#include "MeshFactory.h"

namespace Arcane
{
	MeshFactory* MeshFactory::s_Instance = nullptr;

	MeshFactory* MeshFactory::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new MeshFactory();

		return s_Instance;
	}

	Mesh* MeshFactory::CreatePlane()
	{
		return GetInstance()->CreatePlaneImpl();
	}

	Mesh* MeshFactory::CreatePlaneImpl()
	{
		std::vector<MeshVertex> vertices = {
			{{0.5f, 0.0f, 0.5f},   {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5f, 0.0f, -0.5f},  {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{-0.5f, 0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.0f, 0.5f},  {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};

		std::vector<uint32_t> indices = {
			0, 1, 3,
			1, 2, 3
		};
		
		return new Mesh(vertices, indices);
	}
}