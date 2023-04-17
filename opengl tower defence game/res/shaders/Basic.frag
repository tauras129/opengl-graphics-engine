#version 330 core

layout(location = 0) out vec4 color;

//uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

in vec2 v_TexCoord;
//in float v_TexIndex;
//in vec4 v_Color;

void main()
{
	vec4 texColor = texture(u_Textures[int(1)], v_TexCoord);
	color = texColor;//vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f);
	//color = v_Color;
}