#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_ColorIn = {1.0, 1.0, 1.0, 1.0};

void main()
{
	color = u_ColorIn;
}