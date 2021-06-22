#pragma once

#include "Camera.h"
#include "Model.h"

class Coin {
public:
	void Init();
	void DrawCoin(Shader coinShader, Camera& camera, float deltaTime, unsigned int enumPos);

private:
	float angularSpeed = 0.0f; // coin's rotating speed
	

	Model CoinModel;

};

