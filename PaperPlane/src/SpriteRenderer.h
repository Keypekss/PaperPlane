#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"


class SpriteRenderer {
public:
	// Constructor (inits shaders/shapes)
	SpriteRenderer(Shader &shader);

	void DrawLine(glm::vec3 location, glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color, Camera &camera);
	void DrawBlock(glm::vec3 pos, float width, float height, float depth, glm::vec3 color, Camera& camera);

private:
	// Render state
	Shader shader;
	//Camera camera = Camera(glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
};

