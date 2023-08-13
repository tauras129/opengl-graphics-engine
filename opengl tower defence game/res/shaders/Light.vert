#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoords;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
    v_FragPos = vec3(u_Model * vec4(position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * normal;
    v_TexCoords = texCoord;
}