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

	for (auto block : blocks) {
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
	if(blocks.empty()){
		while(blocks.size() < difficulty){
			Block block;
			block.GenerateBlock(RPos);
			// push_back if generated block is in different position
			if (blocks.end() == std::find_if(blocks.begin(), blocks.end(), [&](Block blk) { return block == blk; })) {				
				unsigned int sameDepth1 = 0; // blocks with same depth value
				unsigned int sameDepth2 = 0; // blocks with same depth value
				unsigned int sameDepth3 = 0; // blocks with same depth value
				for (const auto& block : blocks) {
					if (block.GetPos().z == RPos.z - 4.0f)
						sameDepth1++;
					if (block.GetPos().z == RPos.z - 10.0f)
						sameDepth2++;
					if (block.GetPos().z == RPos.z - 16.0f)
						sameDepth3++;
				}
				// be sure that the path is not completely blocked. (completely blocked path has 9 blocks)
				if (sameDepth1 <= 8 && sameDepth1 <= 8 && sameDepth1 <= 8) {
					blocks.push_back(block);
					blockEnumPos.push_back(block.GetEnumPos());
				}
			}
		}		
	}

	GenerateCoins();
}

void Room::GenerateCoins()
{
	std::vector<unsigned int> allPos = { 0,1,2,3,4,5,6,7,8 }; // all possible enum positions for a block or a coin
	std::vector<unsigned int> coinPos(9); // possible coin positions
	std::vector<unsigned int> selectedPos;

// 	std::cout << "Block Pos: ";
// 	for (auto pos : blockEnumPos)
// 		std::cout << pos << "	";
// 	std::cout << std::endl;
// 	std::cout << std::endl;

	std::sort(blockEnumPos.begin(), blockEnumPos.end());
	std::set_difference(allPos.begin(), allPos.end(), blockEnumPos.begin(), blockEnumPos.begin() + 9, coinPos.begin()); // determine enum positions where there are not blocks
	
// 	std::cout << "coin Pos: ";
// 	for (auto pos : coinPos)
// 		std::cout << pos << "	";
// 	std::cout << std::endl;
																												  
	// get a random coin position from possible coin position vector
	std::sample(
		coinPos.begin(),
		coinPos.end(),
		std::back_inserter(selectedPos),
		1,
		std::mt19937{ std::random_device{}() }
	);
	
	coinPos.push_back(selectedPos.at(0));
//  	std::cout << selectedPos.at(0) << std::endl;
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
	return blocks;
}

Room::~Room()
= default;

