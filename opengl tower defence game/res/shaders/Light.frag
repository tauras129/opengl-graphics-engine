#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material u_Material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light u_Light;

uniform vec3 u_ViewPos;

in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_TexCoords;

void main()
{
    // ambient
    vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));

    // diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));

    // specular
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * vec3(texture(u_Material.specular, v_TexCoords));

    // attenuation
    float distance    = length(u_Light.position - v_FragPos);
    float attenuation = 1.0 / (u_Light.constant + u_Light.linear * distance + u_Light.quadratic * (distance * distance));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;


    vec3 result = u_Light.ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}