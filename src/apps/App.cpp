#include "App.h"

namespace App
{
	AppMenu::AppMenu(AppBase*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{
	}

	void AppMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}
}