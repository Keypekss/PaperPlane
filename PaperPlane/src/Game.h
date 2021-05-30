#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Room.h"
#include "Plane.h"

// initial player values

class Game {
public:
	Game() = default;
	~Game();

	void Init();
	void DrawSkybox();
	void GenerateRooms(Camera &camera);
	void RemoveRoom(Camera &camera);
	void Render(Camera &camera);
	void ProcessInput(float deltaTime, Camera& camera);
	void Update(float deltaTime, Camera& camera);

	bool Keys[1024];


private:
	void Clear();	

	Plane plane;
	unsigned int RoomCount = 5;	
	std::vector<Room> Rooms;	
};

