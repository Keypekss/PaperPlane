#include "Game.h"
#include "SpriteRenderer.h"

SpriteRenderer* LineRenderer;
SpriteRenderer* BlockRenderer;
Shader modelShader;
Shader silhouetteShader;

void Game::Init()
{
	Shader lineShader("Shaders/line.vert", "Shaders/line.frag");
	LineRenderer = new SpriteRenderer(lineShader);

	Shader blockShader("Shaders/block.vert", "Shaders/block.frag");
	BlockRenderer = new SpriteRenderer(blockShader);

	modelShader = Shader("Shaders/modelShader.vert", "Shaders/modelShader.frag");
	silhouetteShader = Shader("Shaders/silhouette.vert", "Shaders/silhouette.frag");

	plane = Plane();
	plane.Init();
}

bool FirstRoomInserted = false;
void Game::GenerateRooms(Camera &camera)
{	
	if (!FirstRoomInserted) {
		Room room{ glm::vec3(0.0f) };
		room.GenerateRoom(18);
		Rooms.push_back(room);
		FirstRoomInserted = true;
	}

	while (Rooms.size() != RoomCount) {
		// construct a room based on the position of the last room in the vector
		Room room{ glm::vec3(0.0f, 0.0f, (*Rooms.rbegin()).GetPos().z - (*Rooms.rbegin()).GetDepth()) };
		room.GenerateRoom(18);
		Rooms.push_back(room);
	}

	for (auto & Room : Rooms) {
		Room.DrawDebugCube(*LineRenderer, *BlockRenderer ,camera);
	}
}

void Game::RemoveRoom(Camera &camera)
{
	// remove the room if its behind the camera
	if (camera.Position.z < Rooms.at(0).GetPos().z - Rooms.at(0).GetDepth()) {
		auto first_it = Rooms.begin();
		Rooms.erase(first_it);
	}
}

void Game::Render(Camera &camera)
{
	GenerateRooms(camera);
	RemoveRoom(camera);
	plane.drawPlane(modelShader, camera);
	plane.drawSilhouette(silhouetteShader, camera);
}

void Game::ProcessInput(float deltaTime, Camera& camera)
{
	// plane controls
	plane.TranslatePlane.y -= 5.0f * deltaTime;
	camera.Position.z -= 5.0f * deltaTime;
	if (Keys[GLFW_KEY_UP])
	{
		plane.TranslatePlane.z += 5.0f * deltaTime;
		camera.Position.y += 5.0f * deltaTime;
	}
	if (Keys[GLFW_KEY_DOWN])
	{
		plane.TranslatePlane.z -= 5.0f * deltaTime;
		camera.Position.y -= 5.0f * deltaTime;
	}
	if (Keys[GLFW_KEY_LEFT])
	{
		plane.TranslatePlane.x += 5.0f * deltaTime;
		camera.Position.x -= 5.0f * deltaTime;
	}
	if (Keys[GLFW_KEY_RIGHT])
	{
		plane.TranslatePlane.x -= 5.0f * deltaTime;
		camera.Position.x += 5.0f * deltaTime;
	}
}

void Game::Update(float deltaTime, Camera& camera)
{
	Render(camera);
	ProcessInput(deltaTime, camera);
}

Game::~Game()
{
	delete LineRenderer;
	delete BlockRenderer;
}

void Game::Clear()
{

}

