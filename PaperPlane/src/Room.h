#pragma once

#include <glm/glm.hpp>

#include "SpriteRenderer.h"
#include "Block.h"

class Room {
public:
	Room();
	Room(glm::vec3 depth);

	void DrawDebugCube(SpriteRenderer& lineRenderer, SpriteRenderer& blockRenderer, Camera &camera);
	void GenerateRoom(unsigned int difficulty);

	// getters
	glm::vec3 GetPos();
	int GetDepth();
	
	~Room();
private:
	std::vector<Block> blocks;

	// room dimensions
	int RWidth, RHeight, RDepth;

	// room position
	glm::vec3 RPos;	
};

