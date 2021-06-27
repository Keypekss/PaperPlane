#pragma once

#include "Camera.h"
#include "Model.h"
#include "SpriteRenderer.h"

class Coin {
public:
	static void Init();
	void GenerateCoin(unsigned int enumPos, float depth);
	void DrawCoin(Shader coinShader, Camera& camera, float deltaTime, float angle);
	void DrawCBox(SpriteRenderer& lineRenderer, Camera& camera);
	glm::vec3 GetCoinPos();
	float CBoxWidth = 0.8f, CBoxHeight = 0.8f, CBoxDepth = 0.8f;

private:
	glm::vec3 coinPos;
	glm::vec3 coinCBPos; // collision box pos
	
	inline static Model CoinModel;
};

