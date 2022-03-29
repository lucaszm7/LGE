#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include "imgui/imgui.h"

namespace Scene
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
		Menu(Scene_t*& currentTestPointer);
		void OnImGuiRender() override;

		template <typename T>
		void RegisterApp(const std::string& name)
		{
			std::cout << "Registering Test: " << name << "\n";
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	public:
		std::string c_SceneName = "Main Menu";
	private:
		Scene_t*& m_CurrentTest;
		std::vector< std::pair< std::string, std::function<Scene_t* ()>>> m_Tests;
	};
}