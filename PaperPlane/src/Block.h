#pragma once

#include "SpriteRenderer.h"

class Block {

public:
	Block();	

	void DrawBlock(SpriteRenderer& blockRenderer, Camera &camera);
	void GenerateBlock(glm::vec3 pos);

	// getters
	glm::vec3 GetPos() const;

	~Block();	

private:
	void RandomizeDepth(glm::vec3 pos);
	// block dimensions
	float BWidth, BHeight, BDepth;

	// block position
	glm::vec3 BPos;

	// block color
	glm::vec3 BColor;

	enum Positions {
		BOTTOM_LEFT,
		BOTTOM_MID,
		BOTTOM_RIGHT,
		CENTER_LEFT,
		CENTER,
		CENTER_RIGHT,
		TOP_LEFT,
		TOP_MID,
		TOP_RIGHT
	};

	enum Depths {
		FRONT,
		MIDDLE,
		BACK
	};	
};

