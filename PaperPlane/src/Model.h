#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

class Model {
public:
	glm::vec3 Pos;
	glm::vec3 Size;

	Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false);

	void loadModel(std::string path);

	void render(Shader shader, bool setModel = true);

	void cleanup();

protected:
	bool NoTex;

	std::vector<Mesh> Meshes;
	std::string Directory;

	std::vector<Texture> Textures_loaded;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type);
};

