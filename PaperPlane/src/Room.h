#pragma once

#include <map>

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
	std::vector<Coin> GetCoins() const;
	~Room();
private:
	std::vector<Block> Blocks;
	std::vector<Coin> Coins;

	// i hate myself for writing this
	std::vector<unsigned int> blockEnumPos4;
	std::vector<unsigned int> blockEnumPos10;
	std::vector<unsigned int> blockEnumPos16;

	// room dimensions
	int RWidth, RHeight, RDepth;

	// room position
	glm::vec3 RPos;	
};

