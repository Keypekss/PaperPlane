#pragma once

#include "Camera.h"
#include "Model.h"

class Coin {
public:
	static void Init();
	void GenerateCoin(unsigned int enumPos, float depth);
	void DrawCoin(Shader coinShader, Camera& camera, float deltaTime);
	
private:
	float angularSpeed = 0.0f; // coin's rotating speed
	glm::vec3 coinPos;
	inline static Model CoinModel;
};

