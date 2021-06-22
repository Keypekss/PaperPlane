#pragma once

#include <glm/glm.hpp>

#include "SpriteRenderer.h"
#include "Coin.h"
#include "Block.h"

class Room {
public:
	Room();
	Room(glm::vec3 depth);

	void DrawDebugCube(SpriteRenderer& lineRenderer, SpriteRenderer& blockRenderer, Camera &camera);
	void GenerateRoom(unsigned int difficulty);
	void GenerateCoins(); // need position of the all generated blocks, that's why this function is implemented here

	// getters
	glm::vec3 GetPos() const;
	int GetDepth() const;
	std::vector<Block> GetBlocks() const;
	std::vector<unsigned int> coinEnumPos;
	~Room();
private:
	std::vector<Block> blocks;
	Coin coin;
	std::vector<unsigned int> blockEnumPos;
	
	// room dimensions
	int RWidth, RHeight, RDepth;

	// room position
	glm::vec3 RPos;	
};

