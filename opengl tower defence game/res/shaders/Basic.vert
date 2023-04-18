#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
//layout(location = 3) in float texIndex;

out vec2 v_TexCoord;
//out float v_TexIndex;
//out vec4 v_Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	mat4 MVP = u_Proj * u_View * u_Model;
	gl_Position = MVP * position;
//	v_Color = color;
//	v_TexIndex = texIndex;
	v_TexCoord = texCoord;
}
