#include "SceneManager.h"

namespace Arcane
{
	SceneManager* SceneManager::s_Instance = nullptr;

	SceneManager* SceneManager::GetInstance()
	{
		if (!s_Instance)
			s_Instance = new SceneManager();

		return s_Instance;
	}

	void SceneManager::Init()
	{
		GetInstance()->InitImpl();
	}

	void SceneManager::InitImpl()
	{
	}

	void SceneManager::AddScene(Scene* scene)
	{
		GetInstance()->AddSceneImpl(scene);
	}

	void SceneManager::AddSceneImpl(Scene* scene)
	{
		m_Scenes[m_SceneCount] = scene;
		m_SceneCount++;
	}

	// Get Current Scene (0 position in map)
	Scene* SceneManager::GetCurrentScene()
	{
		return GetInstance()->GetCurrentSceneImpl();
	}

	Scene* SceneManager::GetCurrentSceneImpl()
	{
		return m_Scenes[m_Current];
	}

	// Get Any Scene by name
	Scene* SceneManager::GetScene(std::string name)
	{
		return GetInstance()->GetSceneImpl(name);
	}

	Scene* SceneManager::GetSceneImpl(std::string name)
	{
		for (const auto& [key, val] : m_Scenes)
		{
			if (val->GetName() == name)
			{
				m_Current = key;
				return val;
			}
		}

		return nullptr;
	}

	// Get Next Scene(Get Current map value + 1)
	Scene* SceneManager::GetNextScene()
	{
		return GetInstance()->GetNextSceneImpl();
	}

	std::map<int, Scene*> SceneManager::GetAllScenes()
	{
		return GetInstance()->GetAllScenesImpl();
	}

	std::map<int, Scene*> SceneManager::GetAllScenesImpl()
	{
		return m_Scenes;
	}

	Scene* SceneManager::GetNextSceneImpl()
	{
		if (m_Current + 1 >= m_Scenes.size())
			return nullptr;

		m_Current += 1;
		return m_Scenes[m_Current];
	}
}