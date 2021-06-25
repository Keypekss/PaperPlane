#pragma once

#include "Camera.h"
#include "Model.h"

class Coin {
public:
	static void Init();
	void GenerateCoin(unsigned int enumPos, float depth);
	void DrawCoin(Shader coinShader, Camera& camera, float deltaTime, float angle);
	
private:
	glm::vec3 coinPos;
	inline static Model CoinModel;
};

