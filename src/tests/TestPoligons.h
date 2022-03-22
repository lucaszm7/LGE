#pragma once

#include "Test.h"
#include <iostream>
#include <vector>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

namespace test
{
	class TestPoligons : public Test
	{
	public:
		TestPoligons(float *v, unsigned int size);
		~TestPoligons();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float *m_Vertices;
	};
}