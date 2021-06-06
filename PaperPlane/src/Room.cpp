#include <vector> 
#include <algorithm>

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

	GenerateRoom(18);
	for (auto block : blocks) {
		block.DrawBlock(blockRenderer ,camera);
		//block.DrawDebugLines(lineRenderer, camera);
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
			if(blocks.end() == std::find_if(blocks.begin(), blocks.end(), [&](Block blk) { return block == blk; }));
				blocks.push_back(block);
		}		
	}
}

glm::vec3 Room::GetPos()
{
	return RPos;
}

int Room::GetDepth()
{
	return RDepth;	
}

std::vector<Block> Room::GetBlocks()
{
	return blocks;
}

Room::~Room()
= default;

