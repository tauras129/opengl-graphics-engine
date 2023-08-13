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
    layout.Push<float>(3); //how many dimensions the thing is

    vb.Set(cubeVertices.data(), (unsigned int)(sizeof(cubeVertices[0]) * cubeVertices.size()));
    va.AddBuffer(vb, layout);
    ib.Set(cubeIndices, (unsigned int)cubeIndices.size());

    //lightShader.Bind();
    //lightShader.SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
    //lightShader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);

    updateModelMatrix();

}

Light::~Light()
{

}
