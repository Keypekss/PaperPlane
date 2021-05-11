#pragma once

#include <glm/glm.hpp>

#include "SpriteRenderer.h"

class Room {
public:
	Room();
	void DrawDebugCube(Camera &camera);

	~Room();
private:
	void DrawBlock(Camera &camera);
	void GenerateBlock();

	// room dimensions
	int RWidth, RHeight, RDepth;

	// room position
	glm::vec3 RPos;

	// block dimensions
	float BWidth, BHeight, BDepth;

	// block position
	glm::vec3 BPos;

	enum Positions {
		BOTTOM_LEFT,
		BOTTOM_MID,
		BOTTOM_RIGHT,
		MIDDLE_LEFT,
		MIDDLE,
		MIDDLE_RIGHT,
		TOP_LEFT,
		TOP_MID,
		TOP_RIGHT
	};

	SpriteRenderer* LineRenderer;
	SpriteRenderer* BlockRenderer;
	
};

