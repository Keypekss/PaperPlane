#include <iostream>

#include "Mesh.h"

std::vector<Vertex> Vertex::genList(float* vertices, int noVertices)
{
	std::vector<Vertex> ret(noVertices);

	int stride = sizeof(Vertex) / sizeof(float);

	for (int i = 0; i < noVertices; i++) {
		ret[i].Position = glm::vec3(
			vertices[i * stride + 0],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
		);

		ret[i].Normal = glm::vec3(
			vertices[i * stride + 3],
			vertices[i * stride + 4],
			vertices[i * stride + 5]
		);

		ret[i].TexCoords = glm::vec2(
			vertices[i * stride + 6],
			vertices[i * stride + 7]
		);

// 		ret[i].Tangent = glm::vec3(
// 			vertices[i * stride + 8],
// 			vertices[i * stride + 9],
// 			vertices[i * stride + 10]
// 		);
// 
// 		ret[i].Bitangent = glm::vec3(
// 			vertices[i * stride + 11],
// 			vertices[i * stride + 12],
// 			vertices[i * stride + 13]
// 		);
	}

	return ret;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: Vertices(vertices), Indices(indices), Textures(textures), NoTex(false)
{
	setup();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular, aiColor4D emissive, aiColor4D transparent, aiColor4D reflective)
	: Vertices(vertices), Indices(indices), Diffuse(diffuse), Specular(specular), Emissive(emissive), Transparent(transparent), Reflective(reflective), NoTex(true)
{
	setup();
}

void Mesh::render(Shader shader)
{
	if (NoTex) {
		// materials
		shader.set4Float("material.diffuse", Diffuse);
		shader.set4Float("material.specular", Specular);
		shader.set4Float("material.emissive", Emissive);
		shader.set4Float("material.transparent", Transparent);
		shader.set4Float("material.reflective", Reflective);
		shader.setInt("noTex", 1);
	} else {
		// textures
		unsigned int diffuseIdx = 0;
		unsigned int specularIdx = 0;

		for (unsigned int i = 0; i < Textures.size(); i++) {
			// activate texture
			glActiveTexture(GL_TEXTURE0 + i);

			// retrieve texture info
			std::string name;
			switch (Textures[i].Type) {
			case aiTextureType_DIFFUSE:
				name = "diffuse" + std::to_string(diffuseIdx++);
				break;
			case aiTextureType_SPECULAR:
				name = "specular" + std::to_string(specularIdx++);
				break;
			}

			// set the shader value
			shader.setInt(name, i);
			// bind texture
			Textures[i].bind();
		}
	}

	// EBO stuff
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	// reset
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::cleanup()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::setup()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO); // vertex array object
	glGenBuffers(1, &VBO); // vertex buffer object
	glGenBuffers(1, &EBO); // element buffer object

	glBindVertexArray(VAO);

	// load data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	// load data into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	// set vertex attribute pointers
	// vertex.position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex.normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	// vertex.texCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));
// 	// vertex.tangent
// 	glEnableVertexAttribArray(3);
// 	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
// 	// vertex.bitangent
// 	glEnableVertexAttribArray(4);
// 	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}