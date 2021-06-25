#include <vector> 
#include <algorithm>
#include <random>

#include "Room.h"

Room::Room()
	: RWidth(9), RHeight(9), RDepth(18), RPos(glm::vec3(0.0f)) 
{
}

Room::Room(glm::vec3 depth)
	: RWidth(9), RHeight(9), RDepth(18), RPos(depth)
{
}

void Room::DrawDebugCube(SpriteRenderer& lineRenderer, SpriteRenderer& blockRenderer, Camera &camera)
{
	std::vector<std::vector<glm::vec3>> vertices {
		// first point								// second point
		{glm::vec3(0,		0,			0),			glm::vec3(RWidth,	0,			0)}, // front
		{glm::vec3(0,		0,			0),			glm::vec3(0,		RHeight,	0)},
		{glm::vec3(0,		RHeight,	0),			glm::vec3(RWidth,	RHeight,	0)},
		{glm::vec3(RWidth,	RHeight,	0),			glm::vec3(RWidth,	0,			0)},
									  			
		{glm::vec3(0,		0,			0),			glm::vec3(0,		0,			-RDepth)}, // sides
		{glm::vec3(0,		RHeight,	0),			glm::vec3(0,		RHeight,	-RDepth)},
		{glm::vec3(RWidth,	RHeight,	0),			glm::vec3(RWidth,	RHeight,	-RDepth)},
		{glm::vec3(RWidth,	0,			0),			glm::vec3(RWidth,	0,			-RDepth)},
		
		{glm::vec3(0,		0,			-RDepth),	glm::vec3(RWidth,	0,			-RDepth)}, // back
		{glm::vec3(0,		0,			-RDepth),	glm::vec3(0,		RHeight,	-RDepth)},
		{glm::vec3(0,		RHeight,	-RDepth),	glm::vec3(RWidth,	RHeight,	-RDepth)},
		{glm::vec3(RWidth,	RHeight,	-RDepth),	glm::vec3(RWidth,	0,			-RDepth)}
	};
	
	for (size_t i = 0; i < vertices.size(); i++) {
		glm::vec3 startPos = vertices.at(i).at(0);
		glm::vec3 endPos = vertices.at(i).at(1);
		lineRenderer.DrawLine(RPos, startPos, endPos, glm::vec3(1.0f, 0.0f, 0.0f), camera);
	}

	for (auto& block : Blocks) {
		block.DrawBlock(blockRenderer ,camera);
	}
}

bool operator==(const Block& lhs, const Block& rhs)
{
	return lhs.GetPos() == rhs.GetPos();
}

void Room::GenerateRoom(unsigned int difficulty)
{	
	// difficulty equals to number of blocks in a room
	if(Blocks.empty()){
		while(Blocks.size() < difficulty){
			Block block;
			block.GenerateBlock(RPos);
			// push_back if generated block is in different position
			if (Blocks.end() == std::find_if(Blocks.begin(), Blocks.end(), [&](Block blk) { return block == blk; })) {				
				unsigned int sameDepth1 = 0; // blocks with same depth value
				unsigned int sameDepth2 = 0;
				unsigned int sameDepth3 = 0;
				for (const auto& block : Blocks) {
					if (block.GetPos().z == RPos.z - 4.0f)
						sameDepth1++;
					if (block.GetPos().z == RPos.z - 10.0f)
						sameDepth2++;
					if (block.GetPos().z == RPos.z - 16.0f)
						sameDepth3++;
				}
				// be sure that the path is not completely blocked. (completely blocked path has 9 blocks)
				if (sameDepth1 <= 8 && sameDepth1 <= 8 && sameDepth1 <= 8) {
					Blocks.push_back(block);
					/*blockEnumPos.push_back(block.GetEnumPos());*/	
					if (block.GetPos().z == RPos.z - 4.0f)
						blockEnumPos4.push_back(block.GetEnumPos());
					if (block.GetPos().z == RPos.z - 10.0f)
						blockEnumPos10.push_back(block.GetEnumPos());
					if (block.GetPos().z == RPos.z - 16.0f)
						blockEnumPos16.push_back(block.GetEnumPos());
				}
			}
		}		
	}

	GenerateCoins();
}

void Room::GenerateCoins()
{
	std::vector<unsigned int> allPos = { 0,1,2,3,4,5,6,7,8 }; // all possible enum positions for a block or a coin
	std::vector<unsigned int> coinPos(9); // possible coin positions (filled with 9 zeros)
	std::vector<unsigned int> selectedPos; // where coin will spawn at
		
	std::sort(blockEnumPos4.begin(), blockEnumPos4.end());		
	auto it = std::set_difference(allPos.begin(), allPos.end(), blockEnumPos4.begin(), blockEnumPos4.end(), coinPos.begin()); // determine enum positions where there are not blocks
	
	coinPos.resize(it - coinPos.begin()); // erase the zeros at the end

	// get a random coin position from possible coin position vector
	std::sample(
		coinPos.begin(),
		coinPos.end(),
		std::back_inserter(selectedPos),
		1,
		std::mt19937{ std::random_device{}() }
	);

	Coin coin;
	coin.GenerateCoin(selectedPos.at(0), RPos.z - 4.0f);
	Coins.push_back(coin);
}

glm::vec3 Room::GetPos() const
{
	return RPos;
}

int Room::GetDepth() const
{
	return RDepth;	
}

std::vector<Block> Room::GetBlocks() const
{
	return Blocks;
}

std::vector<Coin> Room::GetCoins() const
{
	return Coins;
}

Room::~Room()
= default;

