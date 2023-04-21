#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in int texIndex;

out vec2 v_TexCoord;
flat out int v_TexIndex;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	mat4 MVP = u_Proj * u_View * u_Model;
	v_TexIndex = texIndex;
	v_TexCoord = texCoord;
	gl_Position = MVP * position;
}
