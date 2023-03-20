#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;

out vec2 v_TexCoord;
out float v_TexIndex;
out vec4 v_Color;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_Color = color;
	v_TexIndex = texIndex;
	v_TexCoord = texCoord;
}
