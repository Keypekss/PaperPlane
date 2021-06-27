#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Room.h"
#include "Plane.h"

// initial player values

enum Directions {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

class Game {
public:
	Game() = default;
	~Game();

	void Init();
	void InitSkybox();
	void DrawSkybox(Camera& camera);	
	unsigned int loadCubemap(std::vector<std::string> faces);
	void GenerateRooms(Camera &camera);
	void RemoveRoom(Camera &camera);
	void Render(float deltaTime, Camera &camera);
	void ProcessInput(float deltaTime, Camera& camera);
	void Update(float deltaTime, Camera& camera);
	bool CheckCollision(Plane& plane, Block& block);
	bool CheckCollision(Plane& plane, Coin& block);
	void DoCollisions();
	void Move(Camera& camera, float deltaTime, float moveBy, int dir);
	Plane plane;
	Coin coin;
	bool Keys[1024];
	bool KeysProcessed[1024];

private:
	void Clear();	
	
	unsigned int RoomCount = 5;		
	float angularSpeed = 0.0f;
	std::vector<Room> Rooms;
	glm::vec3 previousPlanePos = plane.InitialPlanePos;
};

