#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Room.h"

// initial player values
const float PLAYER_VELOCITY(100.0f);

class Game {
public:
	Game() = default;

	void Init();
	void GenerateRooms(Camera &camera);

	~Game();

private:

	void Clear();
	unsigned int RoomCount = 5;	
	std::vector<Room> Rooms;

};

