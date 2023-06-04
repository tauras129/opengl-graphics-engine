#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <map>
#include <string>
#include "VertexBufferLayout.h"
#include <ASSIMP/scene.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


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
	glm::mat4 modelMatrix; // combined transformation matrix
	glm::vec3 translation = glm::vec3(0);
	glm::quat rotation = glm::quat(glm::vec3(0));
	glm::vec3 scale = glm::vec3(1);
	int texID = 1;
	VertexBufferLayout layout;
	VertexArray va;
	VertexBuffer vb;
	IndexBuffer ib;

	Mesh();
	~Mesh();
	void loadModel(const std::string& path/*, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices*/ );
	void processNode(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void processMesh(const aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	std::vector<Vertex> getVertices() const;
	std::vector<unsigned int, std::allocator<unsigned int>> getIndices() const;
	void GlobalMove(glm::vec3 translation);
	void LocalMove(glm::vec3 translation);
	void Rotate(glm::quat rotation);
	void Scale(glm::vec3 scale);

	void SetPosition(glm::vec3 translation);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 scale);
	void appendToVerticesAndIndices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void setTextureID(int id);

	Mesh(const Mesh &other)
	{
		vertices = other.vertices;
		indices = other.indices;
		modelMatrix = other.modelMatrix;
		translation = other.translation;
		rotation = other.rotation;
		scale = other.scale;
		texID = other.texID;
		layout = other.layout;
		vb.Set(vertices.data(), (unsigned int)(sizeof(vertices[0]) * vertices.size()));
		va.AddBuffer(vb, layout);
		ib.Set(indices, (unsigned int)indices.size());
	}

private:
	static std::map<std::string, std::pair<std::vector<Vertex>, std::vector<unsigned int>>> cachedModels;

	void updateModelMatrix() {
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}

};

//static std::map<std::string, std::pair<std::vector<Vertex>, std::vector<unsigned int>>> cachedModels;