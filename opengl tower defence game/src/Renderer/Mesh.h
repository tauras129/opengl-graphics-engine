#pragma once
#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <string>
#include "Shader.h"
#include <map>
#include <ASSIMP/scene.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	// mesh data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int, std::allocator<unsigned int>> indices;

	void loadModel(const std::string& path/*, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices*/ );
	void processNode(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void processMesh(const aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void appendToVerticesAndIndices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

private:
	static std::map<std::string, std::pair<std::vector<Vertex>, std::vector<unsigned int>>> cachedModels;
};

//static std::map<std::string, std::pair<std::vector<Vertex>, std::vector<unsigned int>>> cachedModels;