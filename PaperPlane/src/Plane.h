#pragma once
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "SpriteRenderer.h"

class Plane {
public:
	//constructors
	Plane();
	~Plane() = default;

	void drawPlane(Shader &modelShader, Camera& camera);
	void drawSilhouette(Shader& silhouetteShader, Camera& camera);
	void drawCollisionBox(SpriteRenderer& lineRenderer, Camera& camera);
	void Init();

	glm::vec3 PlanePos;
	glm::vec3 InitialPlanePos;
	glm::vec3 CBoxPos; // collision box position	
	float CBoxWidth = 1.1f, CBoxHeight = 0.35f, CBoxDepth = 1.0f; // collision box size

private:
	Model PlaneModel;
};

