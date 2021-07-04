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

enum GameState {
	MENU,
	START,
	GAME_OVER
};

class Game {
public:
	Game() = default;
	~Game();

	void Init();
	void InitSkybox();
	void InitFramebuffer(); // custom framebuffer for rendering grayscale
	void DrawSkybox(Camera& camera);	
	unsigned int loadCubemap(std::vector<std::string> faces);
	void GenerateRooms(Camera &camera);
	void RemoveRoom(Camera &camera);
	void Render(float deltaTime, Camera &camera);	
	void RenderMenu();
	void RenderUI();
	void ProcessInput(float deltaTime, Camera& camera);
	void Update(float deltaTime, Camera& camera, GLFWwindow* window);
	bool CheckCollision(Plane& plane, Block& block);
	bool CheckCollision(Plane& plane, Coin& block);
	void DoCollisions();
	void Move(Camera& camera, float deltaTime, float moveBy, int dir);
	void Clear();

	Plane plane;
	Coin coin;
	bool Keys[1024];
	bool KeysProcessed[1024];

private:	
	unsigned int RoomCount = 5;	
	unsigned int Width = 1280, Height = 720;
	float Score = 0;
	unsigned int postprocessingVAO, postprocessingVBO, framebuffer, textureColorbuffer;
	float angularSpeed = 0.0f;	
	GameState State;
	std::vector<Room> Rooms;
	glm::vec3 previousPlanePos = plane.InitialPlanePos;
};

