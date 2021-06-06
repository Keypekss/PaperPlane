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
	void InitSkybox();
	void DrawSkybox(Camera& camera);
	unsigned int loadCubemap(std::vector<std::string> faces);
	void GenerateRooms(Camera &camera);
	void RemoveRoom(Camera &camera);
	void Render(Camera &camera);
	void ProcessInput(float deltaTime, Camera& camera);
	void Update(float deltaTime, Camera& camera);
	bool CheckCollision(Plane& plane, Block& block);
	void DoCollisions();
	Plane plane;


	bool Keys[1024];


private:
	void Clear();	

	unsigned int RoomCount = 5;		
	std::vector<Room> Rooms;	
};

