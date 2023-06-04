#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Textures[9];

in vec2 v_TexCoord;
flat in int v_TexIndex;

void main()
{
	vec4 texColor = texture(u_Textures[v_TexIndex], v_TexCoord);
	color = texColor;  //vec4(((sin(texColor.rgb*407))+1)/2, 1.0f);  //vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f);
}