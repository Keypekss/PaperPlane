#include <vector> 

#include "Room.h"

Room::Room()
	: RWidth(9), RHeight(9), RDepth(18), RPos(glm::vec3(0.0f)), BWidth(1.0f), BHeight(1.0f), BDepth(0.1f), BPos(glm::vec3(0.0f))
{
	BWidth = (RWidth / 3) / 2.0f;
	BHeight = (RHeight / 3) / 2.0f;
	BPos.x = RPos.x + BWidth;
	BPos.y = RPos.y + BHeight;	
}

void Room::DrawDebugCube(Camera &camera)
{
	std::vector<std::vector<glm::vec3>> vertices {
		// first point							// second point
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

	Shader lineShader("Shaders/line.vert", "Shaders/line.frag");
	LineRenderer = new SpriteRenderer(lineShader);
	for (size_t i = 0; i < vertices.size(); i++) {		
			glm::vec3 startPos = vertices.at(i).at(0);
			glm::vec3 endPos = vertices.at(i).at(1);
			LineRenderer->DrawLine(RPos, startPos, endPos, glm::vec3(1.0f, 0.0f, 0.0f), camera);
	}

	DrawBlock(camera);
}

void Room::DrawBlock(Camera &camera)
{
	Shader blockShader("Shaders/block.vert", "Shaders/block.frag");
	BlockRenderer = new SpriteRenderer(blockShader);
	GenerateBlock();
	BlockRenderer->DrawBlock(BPos, BWidth, BHeight, BDepth, glm::vec3(1.0f), camera);
}

void Room::GenerateBlock()
{
	int selectPos = rand() % 9;
	std::cout << " num generated: " << selectPos << std::endl;
	switch (selectPos)
	{
		// don't know why i need to multiple by 2 and subtract the width and height.
		// but sometimes it just works -Todd Howard
	case BOTTOM_LEFT:
		BPos.x = (RPos.x + BWidth)  * 1 * 2 - BWidth;
		BPos.y = (RPos.y + BHeight) * 1 * 2 - BHeight;
		break;							   
	case BOTTOM_MID:					   
		BPos.x = (RPos.x + BWidth)	* 2 * 2 - BWidth;
		BPos.y = (RPos.y + BHeight) * 1 * 2 - BHeight;
		break;						   
	case BOTTOM_RIGHT:				   
		BPos.x = (RPos.x + BWidth)	* 3 * 2 - BWidth;
		BPos.y = (RPos.y + BHeight) * 1 * 2 - BHeight;
		break;						   
	case MIDDLE_LEFT:				   
		BPos.x = (RPos.x + BWidth)	* 1 * 2 - BWidth;
		BPos.y = (RPos.y + BHeight) * 2 * 2 - BHeight;
		break;						   
	case MIDDLE:					   
		BPos.x = (RPos.x + BWidth)  * 2 * 2 - BWidth;
		BPos.y = (RPos.y + BHeight) * 2 * 2 - BHeight;
		break;						   
	case MIDDLE_RIGHT:				   
		BPos.x = (RPos.x + BWidth)	* 3 * 2 - BWidth;
		BPos.y = (RPos.y + BHeight) * 2 * 2 - BHeight;
		break;						   
	case TOP_LEFT:					   
		BPos.x = (RPos.x + BWidth)	* 1 * 2 - BWidth;
		BPos.y = (RPos.y + BHeight) * 3 * 2 - BHeight;
		break;						   
	case TOP_MID:					   
		BPos.x = (RPos.x + BWidth)	* 2 * 2 - BWidth;
		BPos.y = (RPos.y + BHeight) * 3 * 2 - BHeight;
		break;						   
	case TOP_RIGHT:					   
		BPos.x = (RPos.x + BWidth)	* 3 * 2 - BWidth;
		BPos.y = (RPos.y + BHeight) * 3 * 2 - BHeight;
		break;
	default:
		std::cout << "Error during selecting obstacle position" << std::endl;
		break;
	}
}

Room::~Room()
{
	delete LineRenderer;
	delete BlockRenderer;
}