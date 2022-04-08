#pragma once
#include "Scene.h"

#include "Renderer.h"
#include "imgui/imgui.h"


namespace Scene
{
	class TestClearColor : public Scene_t
	{
	public:

		TestClearColor()
			:m_ClearColor{ 0.f, 0.f, 0.25f, 1.f }
		{}

		~TestClearColor() {}

		void OnUpdate(float deltaTime) override {}
		void OnRender() override
		{
			glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void OnImGuiRender() override
		{
			ImGui::ColorEdit4("Clear Color", &m_ClearColor[0]);
		}

	private:
		float m_ClearColor[4];
	};
}