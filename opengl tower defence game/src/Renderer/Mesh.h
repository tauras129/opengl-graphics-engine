#pragma once
#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <string>
#include "Shader.h"
#include <map>
#include <ASSIMP/scene.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	int TexID;
};

class Mesh
{
public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int, std::allocator<unsigned int>> indices;
	glm::mat4 prevModelMatrix; // previous combined transformation matrix
	glm::mat4 modelMatrix; // combined transformation matrix
	glm::vec3 translation = glm::vec3(0);
	glm::quat rotation = glm::quat(glm::vec3(0));
	glm::vec3 scale = glm::vec3(0);

	void loadModel(const std::string& path/*, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices*/ );
	void processNode(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void processMesh(const aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	std::vector<Vertex> getVertices();
	std::vector<unsigned int, std::allocator<unsigned int>> getIndices();
	void appendToVerices(std::vector<Vertex>& vertices);
	void appendToIndices(std::vector<unsigned int, std::allocator<unsigned int>>& indices);
	void GlobalMove(glm::vec3 translation);
	void LocalMove(glm::vec3 translation);
	void Rotate(glm::quat rotation);
	void Scale(glm::vec3 scale);

	void SetPosition(glm::vec3 translation);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 scale);
	void appendToVerticesAndIndices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void setTextureID(int id);

private:
	static std::map<std::string, std::pair<std::vector<Vertex>, std::vector<unsigned int>>> cachedModels;

	void updateModelMatrix() {
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
		prevModelMatrix = modelMatrix;
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}

	void applyTransformations() {
		if (modelMatrix != prevModelMatrix)
		{
			for (Vertex& vertex : vertices) {
				glm::vec4 pos = glm::vec4(vertex.Position, 1.0f);
				glm::vec4 norm = glm::vec4(vertex.Normal, 0.0f);
				
				if ( prevModelMatrix != glm::mat4( 0 ))
				{
					vertex.Position = glm::vec3(prevModelMatrix / pos);
					vertex.Normal = glm::vec3(prevModelMatrix / norm);
				}

				pos = modelMatrix * pos;
				norm = modelMatrix * norm;

				vertex.Position = glm::vec3(pos);
				vertex.Normal = glm::vec3(norm);
			}
		}
	}
};

//static std::map<std::string, std::pair<std::vector<Vertex>, std::vector<unsigned int>>> cachedModels;