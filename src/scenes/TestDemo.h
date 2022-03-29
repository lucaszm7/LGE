#pragma once

#include "Scene.h"

#include "GLCore.h"

namespace Scene
{
	class TestDemo : public Scene_t
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