#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include "imgui/imgui.h"

namespace LGE
{
	class Scene_t
	{
	public:
		Scene_t() {}
		virtual ~Scene_t() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class Menu : public Scene_t
	{
	public:
		std::string c_SceneName = "Main Menu";
	private:
		Scene_t*& m_CurrentScene;
		std::vector< std::pair< std::string, std::function<Scene_t* ()>>> m_Scenes;
	private:
		friend class Application;
	public:
		Menu(Scene_t*& currentTestPointer)
			:m_CurrentScene(currentTestPointer)
		{
		}

		void OnImGuiRender() override
		{
			for (auto& scene : m_Scenes)
			{
				if (ImGui::Button(scene.first.c_str()))
				{
					m_CurrentScene = scene.second();
					c_SceneName = scene.first;
				}
			}
		}

		template <typename T>
		void RegisterApp(const std::string& name)
		{
			std::cout << "Registering Test: " << name << "\n";
			m_Scenes.push_back(std::make_pair(name, []() { return new T(); }));
		}

	
	};
}