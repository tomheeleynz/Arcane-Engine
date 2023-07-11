#pragma once

#include <iostream>
#include "Texture.h"

namespace Arcane
{
	class SkyBox
	{
	public:

		static SkyBox* Create(std::vector<Texture*> faces);
	private:
	
	};
}