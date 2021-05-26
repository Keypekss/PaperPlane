#include "Game.h"
#include "SpriteRenderer.h"

SpriteRenderer* LineRenderer;
SpriteRenderer* BlockRenderer;


void Game::Init()
{
	Shader lineShader("Shaders/line.vert", "Shaders/line.frag");
	LineRenderer = new SpriteRenderer(lineShader);

	Shader blockShader("Shaders/block.vert", "Shaders/block.frag");
	BlockRenderer = new SpriteRenderer(blockShader);
}

void Game::GenerateRooms(Camera &camera)
{
	while (Rooms.size() != RoomCount) {
		if (Rooms.size() == 0) {
			Room room{ glm::vec3(0.0f) };
			room.GenerateRoom(18);
			Rooms.push_back(room);
		} else if (Rooms.size() == 1) {
			Room room{ glm::vec3(0.0f, 0.0f, -18.0f) };
			room.GenerateRoom(18);
			Rooms.push_back(room);
		} else if (Rooms.size() == 2) {
			Room room{ glm::vec3(0.0f, 0.0f, -36.0f) };
			room.GenerateRoom(18);
			Rooms.push_back(room);
		} else if (Rooms.size() == 3) {
			Room room{ glm::vec3(0.0f, 0.0f, -54.0f) };
			room.GenerateRoom(18);
			Rooms.push_back(room);
		} else if (Rooms.size() == 4) {
			Room room{ glm::vec3(0.0f, 0.0f, -72.0f) };
			room.GenerateRoom(18);
			Rooms.push_back(room);
		}
	}

	for (auto & Room : Rooms) {
		Room.DrawDebugCube(*LineRenderer, *BlockRenderer ,camera);
	}
}

Game::~Game()
{
	delete LineRenderer;
	delete BlockRenderer;
}

void Game::Clear()
{

}

