#pragma once

#include "Test.h"

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
	class TestDemo : public Test
	{
	public:
		TestDemo();
		~TestDemo();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	public:

		float positions[16];
		unsigned int indices[6];
		VertexBuffer vertexbuffer;
		IndexBuffer indexbuffer;
		Shader shader;
		Texture texture;
		glm::vec3 translation1;
		glm::vec3 translation2;
		bool show_demo_window = false;
		Renderer renderer;
		VertexArray VAO;
		VertexBufferLayout layout;
		glm::mat4 projOrtho;
		glm::mat4 view;
		ImVec4 obj_color;
	};
}