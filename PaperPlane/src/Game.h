#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Room.h"

// initial player values

class Game {
public:
	Game() = default;

	void Init();
	void GenerateRooms(Camera &camera);
	void RemoveRoom(Camera &camera);

	~Game();

private:

	void Clear();
	unsigned int RoomCount = 5;	
	std::vector<Room> Rooms;

};

