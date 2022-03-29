#pragma once
#include "Scene.h"

namespace Scene
{
	class TestClearColor : public Scene_t
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];
	};
}