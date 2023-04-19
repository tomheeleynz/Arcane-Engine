#pragma once
#include <string>
#include <filesystem>

#include "Arcane/Assets/Asset.h"

namespace Arcane
{
	class Script : public Asset
	{
	public:
		Script(std::string name);

		void OnStart();
		void OnUpdate(float deltaTime);
	private:
	};
}