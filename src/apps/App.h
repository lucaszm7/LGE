#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include "imgui/imgui.h"

namespace App
{
	class AppBase
	{
	public:
		AppBase() {}
		virtual ~AppBase() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class AppMenu : public AppBase
	{
	public:
		AppMenu(AppBase*& currentTestPointer);
		void OnImGuiRender() override;

		template <typename T>
		void RegisterApp(const std::string& name)
		{
			std::cout << "Registering Test: " << name << "\n";
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		AppBase*& m_CurrentTest;
		std::vector< std::pair< std::string, std::function<AppBase* ()>>> m_Tests;

	};

}