#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
// 	glm::vec3 Tangent;
// 	glm::vec3 Bitangent;

	static std::vector<Vertex> genList(float* vertices, int noVertices);
};

class Mesh {
public:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	unsigned int VAO;

	std::vector<Texture> Textures;
	aiColor4D Diffuse;
	aiColor4D Specular;
	aiColor4D Emissive;
	aiColor4D Transparent;
	aiColor4D Reflective;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures = {});
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D spec, aiColor4D emissive, aiColor4D transparent, aiColor4D reflective);

	void render(Shader shader);

	void cleanup();

private:
	unsigned int VBO, EBO;

	bool NoTex;

	void setup();
};