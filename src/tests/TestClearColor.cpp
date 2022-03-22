#include "TestClearColor.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test
{
	TestClearColor::TestClearColor()
		:m_ClearColor{ 0.f, 0.f, 0.25f, 1.f }
	{

	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(float deltaTime)
	{
	}

	void TestClearColor::OnRender()
	{
		glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::Begin("test");
		ImGui::ColorEdit4("Clear Color", &m_ClearColor[0]);
		ImGui::End();
	}


}