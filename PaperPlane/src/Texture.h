#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/scene.h>

class Texture {
public:
	Texture(std::string dir, std::string path, aiTextureType type);

	void generate();
	void load(bool flip = true);

	void bind();

	// texture object
	unsigned int ID;
	aiTextureType Type;
	std::string Dir;
	std::string Path;
};