#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

uniform mat4 u_MVP;
uniform vec4 u_Color = {1.0f, 0.0f, 0.0f, 1.0f};


void main()
{
    gl_Position = u_MVP * position;
    v_Color = u_Color;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
}
