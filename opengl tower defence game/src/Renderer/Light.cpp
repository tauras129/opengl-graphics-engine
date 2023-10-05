#include "Light.h"

const std::vector<unsigned int> cubeIndices {
    //Top
    2, 6, 7,
    2, 3, 7,

    //Bottom
    0, 4, 5,
    0, 1, 5,

    //Left
    0, 2, 6,
    0, 4, 6,

    //Right
    1, 3, 7,
    1, 5, 7,

    //Front
    0, 2, 3,
    0, 1, 3,

    //Back
    4, 6, 7,
    4, 5, 7
};


const std::vector<float> cubeVertices {
   -1, -1, 1, //0
    1, -1, 1, //1
   -1,  1, 1, //2
    1,  1, 1, //3
   -1, -1,-1, //4
    1, -1,-1, //5
   -1,  1,-1, //6
    1,  1,-1  //7
};

Light::Light()
{
    // setup cube
    layout.Push<float>(3); //how many dimensions the thing is

    vb.Set(cubeVertices.data(), (unsigned int)(sizeof(cubeVertices[0]) * cubeVertices.size()));
    va.AddBuffer(vb, layout);
    ib.Set(cubeIndices, (unsigned int)cubeIndices.size());

    // set defaults for light properties
    lightProperties.position = glm::vec3(0.0f, 0.0f, 0.0f);

    lightProperties.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    lightProperties.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    lightProperties.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    lightProperties.constant = 1.0f; // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation for examples
    lightProperties.linear = 0.09f;
    lightProperties.quadratic = 0.032f;

    //lightShader.Bind();
    //lightShader.SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
    //lightShader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);

    updateModelMatrix();

}

Light::~Light()
{

}

void Light::SetValues(Shader& shader) const
{
    shader.SetUniform1f("u_Light.constant", GetProperties().constant);
    shader.SetUniform1f("u_Light.linear", GetProperties().linear);
    shader.SetUniform1f("u_Light.quadratic", GetProperties().quadratic);

    shader.SetUniform3f("u_Light.position", GetTranslation().x, GetTranslation().y, GetTranslation().z);
    shader.SetUniform3f("u_Light.ambient", GetProperties().ambient.x, GetProperties().ambient.y, GetProperties().ambient.z);
    shader.SetUniform3f("u_Light.diffuse", GetProperties().diffuse.x, GetProperties().diffuse.y, GetProperties().diffuse.z);
    shader.SetUniform3f("u_Light.specular", GetProperties().specular.x, GetProperties().specular.y, GetProperties().specular.z);

}

void Light::SetValues(Shader& shader, const Camera& camera) const
{
    shader.SetUniform1f("u_Light.constant", GetProperties().constant);
    shader.SetUniform1f("u_Light.linear", GetProperties().linear);
    shader.SetUniform1f("u_Light.quadratic", GetProperties().quadratic);

    shader.SetUniform3f("u_Light.position", GetTranslation().x, GetTranslation().y, GetTranslation().z);
    shader.SetUniform3f("u_Light.ambient", GetProperties().ambient.x, GetProperties().ambient.y, GetProperties().ambient.z);
    shader.SetUniform3f("u_Light.diffuse", GetProperties().diffuse.x, GetProperties().diffuse.y, GetProperties().diffuse.z);
    shader.SetUniform3f("u_Light.specular", GetProperties().specular.x, GetProperties().specular.y, GetProperties().specular.z);

    shader.SetUniform3f("u_ViewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    shader.SetUniformMat4f("u_View", camera.GetViewMatrix());	// give shader the view matrix

}

void Light::SetValues(Shader& shader, const glm::mat4& projectionMatrix) const
{
    shader.SetUniform1f("u_Light.constant", GetProperties().constant);
    shader.SetUniform1f("u_Light.linear", GetProperties().linear);
    shader.SetUniform1f("u_Light.quadratic", GetProperties().quadratic);

    shader.SetUniform3f("u_Light.position", GetTranslation().x, GetTranslation().y, GetTranslation().z);
    shader.SetUniform3f("u_Light.ambient", GetProperties().ambient.x, GetProperties().ambient.y, GetProperties().ambient.z);
    shader.SetUniform3f("u_Light.diffuse", GetProperties().diffuse.x, GetProperties().diffuse.y, GetProperties().diffuse.z);
    shader.SetUniform3f("u_Light.specular", GetProperties().specular.x, GetProperties().specular.y, GetProperties().specular.z);

    shader.SetUniformMat4f("u_Proj", projectionMatrix);			// give shader the projection matrix

}

void Light::SetValues(Shader& shader, const Camera& camera, const glm::mat4& projectionMatrix) const
{
    shader.SetUniform1f("u_Light.constant", GetProperties().constant);
    shader.SetUniform1f("u_Light.linear", GetProperties().linear);
    shader.SetUniform1f("u_Light.quadratic", GetProperties().quadratic);

    shader.SetUniform3f("u_Light.position", GetTranslation().x, GetTranslation().y, GetTranslation().z);
    shader.SetUniform3f("u_Light.ambient", GetProperties().ambient.x, GetProperties().ambient.y, GetProperties().ambient.z);
    shader.SetUniform3f("u_Light.diffuse", GetProperties().diffuse.x, GetProperties().diffuse.y, GetProperties().diffuse.z);
    shader.SetUniform3f("u_Light.specular", GetProperties().specular.x, GetProperties().specular.y, GetProperties().specular.z);

    shader.SetUniform3f("u_ViewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    shader.SetUniformMat4f("u_View", camera.GetViewMatrix());	// give shader the view matrix
    shader.SetUniformMat4f("u_Proj", projectionMatrix);			// give shader the projection matrix

}
