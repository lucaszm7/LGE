#include "Scene.h"

namespace Scene
{
	Menu::Menu(Scene_t*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{
	}

	void Menu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_CurrentTest = test.second();
				c_SceneName = test.first;
			}
		}
	}
}