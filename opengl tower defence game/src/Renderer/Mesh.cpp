#include "Mesh.h"
#include <vector>
#include <GL/glew.h>
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include <stdexcept>
#include <iostream>


std::map<std::string, std::pair<std::vector<Vertex>, std::vector<unsigned int>>> Mesh::cachedModels;

Mesh::Mesh()
{
	layout.Push<float>(3); //how many dimensions the thing is
	layout.Push<float>(3); //normal
	layout.Push<float>(2); //texture coordinates
	layout.Push<unsigned int>(1); //texture ID

	updateModelMatrix();
}

Mesh::~Mesh()
{

}

void Mesh::loadModel(const std::string& path/*, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices*/)
{

	// Check if the model is already in the cache
	auto iter = Mesh::cachedModels.find(path);
	if (iter != Mesh::cachedModels.end()) {
		vertices = iter->second.first;
		indices = iter->second.second;
		return;
	}

	// Create an instance of the Importer class
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_TransformUVCoords);

	// Check if there was an error while loading the file
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Model loading error: " + std::string(importer.GetErrorString()) + " trying to load model: " + path;
		throw std::runtime_error("Model loading error: " + std::string(importer.GetErrorString()) + " trying to load model: " + path);
	}

	// Check if the model contains any meshes
	if (scene->mNumMeshes == 0) {
		throw std::length_error("Model loading error: No meshes found in model: " + path);
	}

	// Process the loaded model
	processNode(scene->mRootNode, scene, vertices, indices);
	//for (unsigned int i = 0; i < vertices.size(); ++i) {

		//std::cout << "vertex out" << std::endl;
		//std::cout << vertices[i].Position.x << ", " << vertices[i].Position.y << ", " << vertices[i].Position.z << std::endl;
	//}
	Mesh::cachedModels.insert(std::make_pair(path, std::make_pair(vertices, indices)));

	vb.Set(vertices.data(), (unsigned int)(sizeof(vertices[0]) * vertices.size()));
	va.AddBuffer(vb, layout);
	ib.Set(indices, (unsigned int)indices.size());
}

void Mesh::processNode(const aiNode* node, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene, vertices, indices);
	}

	// Recursively process all the node's children
	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene, vertices, indices);
	}
}

void Mesh::processMesh(const aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{

	// Check if the mesh has vertices
	if (mesh->mNumVertices == 0) {
		throw std::length_error("Model loading error: Mesh contains no vertices");
	}
	// Process the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		// Set vertex.Position, vertex.Normal, vertex.TexCoords, and vertex.TexID based on mesh->mVertices[i], mesh->mColors[i], mesh->mTextureCoords[i], and mesh->mMaterialIndex, respectively
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0, 0, 0);
		vertex.TexCoords = mesh->HasTextureCoords(0) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0, 0);
		vertex.TexID = 1;

		//std::cout << "vertex" << std::endl;

		vertices.push_back(vertex);
		//std::cout << vertices[i].TexCoords.x << ", " << vertices[i].TexCoords.y << std::endl; // print out texture coordinates
	}

	// Check if the mesh has indices
	if (mesh->mNumFaces == 0 || mesh->mFaces[0].mNumIndices == 0) {
		throw std::length_error("Model loading error: Mesh contains no indices");
	}

	// Process the mesh's indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			//std::cout << "index" << std::endl;
			indices.push_back(face.mIndices[j]);
			//std::cout << indices[j] << std::endl;
		}
	}
}

std::vector<Vertex> Mesh::GetVertices() const
{
	return this->vertices;
}

std::vector<unsigned int, std::allocator<unsigned int>> Mesh::GetIndices() const
{
	return this->indices;
}


void Mesh::GlobalMove(glm::vec3 translation)
{
	this->translation += translation;
	updateModelMatrix();
}

void Mesh::LocalMove(glm::vec3 translation)
{
	this->translation += rotation*translation;
	updateModelMatrix();
}

void Mesh::Rotate(glm::quat rotation)
{
	this->rotation *= rotation;
	updateModelMatrix();
}

void Mesh::Scale(glm::vec3 scale)
{
	this->scale *= scale;
	updateModelMatrix();
}

void Mesh::SetPosition(glm::vec3 translation)
{
	this->translation = translation;
	updateModelMatrix();
}

void Mesh::SetRotation(glm::quat rotation)
{
	this->rotation = rotation;
	updateModelMatrix();
}

void Mesh::SetScale(glm::vec3 scale)
{
	this->scale = scale;
	updateModelMatrix();
}

void Mesh::AppendToVerticesAndIndices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	vertices.insert(vertices.end(), this->vertices.begin(), this->vertices.end());
	indices.insert(indices.end(), this->indices.begin(), this->indices.end());
}

void Mesh::SetTextureID(int id)
{
	this->texID = id;
	for (auto& vertex : vertices) {
		vertex.TexID = id;
	}

	vb.Set(vertices.data(), (unsigned int)(sizeof(vertices[0]) * vertices.size()));
	va.AddBuffer(vb, layout);
}
