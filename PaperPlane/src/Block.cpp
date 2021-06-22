#include "Room.h"
#include "Block.h"

#include <chrono>
#include <random>

Block::Block()
	: BWidth(1.5f), BHeight(1.5f), BDepth(0.1f), BPos(glm::vec3(0.0f)), BColor(1.0f)
{

}

void Block::DrawBlock(SpriteRenderer& blockRenderer, Camera &camera)
{
	blockRenderer.DrawBlock(BPos, BWidth, BHeight, BDepth, BColor, camera);	
}

void Block::GenerateBlock(glm::vec3 pos)
{	
	// pick a random number from the following vector, store in selectedPos
	std::vector<unsigned int> allPos = { 0,1,2,3,4,5,6,7,8 };
	std::vector<unsigned int> selectedPos;
	std::sample(
		allPos.begin(),
		allPos.end(),
		std::back_inserter(selectedPos),
		1,
		std::mt19937{ std::random_device{}() }
	);
	
	enumPos = selectedPos.at(0);
	switch (enumPos) {
		// don't know why i need to multiple by 2 and subtract the width and height.
		// but sometimes it just works -Todd Howard
	case BOTTOM_LEFT:
		BPos.x = pos.x + BWidth * 1 * 2 - BWidth;
		BPos.y = pos.y + BHeight * 1 * 2 - BHeight;
		RandomizeDepth(pos);
		break;
	case BOTTOM_MID:
		BPos.x = pos.x + BWidth * 2 * 2 - BWidth;
		BPos.y = pos.y + BHeight * 1 * 2 - BHeight;
		RandomizeDepth(pos);
		break;
	case BOTTOM_RIGHT:
		BPos.x = pos.x + BWidth * 3 * 2 - BWidth;
		BPos.y = pos.y + BHeight * 1 * 2 - BHeight;
		RandomizeDepth(pos);
		break;
	case CENTER_LEFT:
		BPos.x = pos.x + BWidth * 1 * 2 - BWidth;
		BPos.y = pos.y + BHeight * 2 * 2 - BHeight;
		RandomizeDepth(pos);
		break;
	case CENTER:
		BPos.x = pos.x + BWidth * 2 * 2 - BWidth;
		BPos.y = pos.y + BHeight * 2 * 2 - BHeight;
		RandomizeDepth(pos);
		break;
	case CENTER_RIGHT:
		BPos.x = pos.x + BWidth * 3 * 2 - BWidth;
		BPos.y = pos.y + BHeight * 2 * 2 - BHeight;
		RandomizeDepth(pos);
		break;
	case TOP_LEFT:
		BPos.x = pos.x + BWidth * 1 * 2 - BWidth;
		BPos.y = pos.y + BHeight * 3 * 2 - BHeight;
		RandomizeDepth(pos);
		break;
	case TOP_MID:
		BPos.x = pos.x + BWidth * 2 * 2 - BWidth;
		BPos.y = pos.y + BHeight * 3 * 2 - BHeight;
		RandomizeDepth(pos);
		break;
	case TOP_RIGHT:
		BPos.x = pos.x + BWidth * 3 * 2 - BWidth;
		BPos.y = pos.y + BHeight * 3 * 2 - BHeight;
		RandomizeDepth(pos);
		break;
	default:
		std::cout << "Error during selecting block position" << std::endl;
		break;
	}
}

glm::vec3 Block::GetPos() const
{
	return BPos;
}

unsigned int Block::GetEnumPos() const
{
	return enumPos;
}

void Block::RandomizeDepth(glm::vec3 pos)
{
	int selectDepth = rand() % 3;
	switch (selectDepth) {
	case FRONT:
		BColor = glm::vec3(0.0f, 1.0f, 1.0f);
		BPos.z = pos.z - 4.0f;
		break;
	case MIDDLE:
		BColor = glm::vec3(1.0f, 1.0f, 0.0f);
		BPos.z = pos.z - 10.0f;
		break;
	case BACK:
		BColor = glm::vec3(0.0f, 1.0f, 0.0f);
		BPos.z = pos.z - 16.0f;
		break;
	}
}

Block::~Block()
= default;