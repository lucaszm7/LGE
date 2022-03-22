#include "TestDemo.h"

test::TestDemo::TestDemo()
    : 
        positions
            {
                // Vertex attrib
                //   POS     //   UV
                -50.0f, -50.0f, 0.0f, 0.0f, // 0 // This is a vertex
                50.0f, -50.0f, 1.0f, 0.0f, // 1 // Another vertex
                50.0f, 50.0f, 1.0f, 1.0f, // 2 // yet another vertex
                -50.0f, 50.0f, 0.0f, 1.0f, // 3 // and another vertex
            },
        indices
            {
                0, 1, 2,
                2, 3, 0,
            },

        // nº of vertex * size of each vertex
        vertexbuffer(positions, 4 * 4 * sizeof(float)),
        indexbuffer(indices, 6),
        shader("res/shaders/Basic.shader"),
        texture("res/textures/goblin-12.png"),
        translation1(200.0f, 200.0f, 0.0f),
        translation2(200.0f, 200.0f, 0.0f)
{



    layout.Push<float>(2);
    layout.Push<float>(2);
    VAO.AddBuffer(vertexbuffer, layout);

    projOrtho = glm::ortho(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -1.0f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    shader.Bind();
    texture.Bind();
    // We need to tell our shader, witch texture slot to sample from
    shader.SetUniform1i("u_Texture", 0);

    // Unbind everthing
    VAO.Unbind();
    vertexbuffer.Unbind();
    indexbuffer.Unbind();
    shader.Unbind();

    obj_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

test::TestDemo::~TestDemo()
{
    // Unbind everthing
    VAO.Unbind();
    vertexbuffer.Unbind();
    indexbuffer.Unbind();
    shader.Unbind();
    texture.Unbind();
}

void test::TestDemo::OnUpdate(float deltaTime)
{

}

void test::TestDemo::OnRender()
{
    shader.Bind();

    {
        // Object Transform = Translation, Rotation, Scale3
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation1);
        glm::mat4 mvp = projOrtho * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniform4f("u_Color", obj_color.x * obj_color.w, obj_color.y * obj_color.w, obj_color.z * obj_color.w, obj_color.w);
        renderer.Draw(VAO, indexbuffer, shader);
    }

    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation2);
        glm::mat4 mvp = projOrtho * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniform4f("u_Color", obj_color.x * obj_color.w, obj_color.y * obj_color.w, obj_color.z * obj_color.w, obj_color.w);
        renderer.Draw(VAO, indexbuffer, shader);
    }
}

void test::TestDemo::OnImGuiRender()
{
    {
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::SliderFloat3("Translation - 1", &translation1.x, 0.0f, (float)SCREEN_WIDTH);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat3("Translation - 2", &translation2.x, 0.0f, (float)SCREEN_WIDTH);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::NewLine();
        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::NewLine();
        ImGui::ColorEdit3("Obj color", (float*)&obj_color); // Edit 3 floats representing a color
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    }
}
