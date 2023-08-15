#pragma once

#include <map>
#include <string>

#include "Scene.h"

namespace Arcane
{
	class SceneManager
	{
	public:
		static void Init();
		static void AddScene(Scene* scene);
		static SceneManager* GetInstance();

		// Access Scene Methods

		// Get Current Scene (0 position in map)
		static Scene* GetCurrentScene();

		// Get Any Scene by name
		static Scene* GetScene(std::string name);
		
		// Get Next Scene(Get Current map value + 1)
		static Scene* GetNextScene();

		// Get All Scenes
		static std::map<int, Scene*> GetAllScenes();
	private:
		SceneManager() = default;

		void InitImpl();
		void AddSceneImpl(Scene* scene);

		Scene* GetCurrentSceneImpl();
		Scene* GetSceneImpl(std::string name);
		Scene* GetNextSceneImpl();

		std::map<int, Scene*> GetAllScenesImpl();
	private:
		static SceneManager* s_Instance;
		std::map<int, Scene*> m_Scenes;
		int m_SceneCount = 0;
		int m_Current = 0;
	};
}