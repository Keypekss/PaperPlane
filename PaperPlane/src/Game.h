#pragma once

#include "Room.h"

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

