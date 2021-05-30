#pragma once
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

class Plane {
public:
	//constructors
	Plane();
	~Plane() = default;

	void drawPlane(Shader &modelShader, Camera& camera);
	void drawSilhouette(Shader& silhouetteShader, Camera& camera);
	void setTranslatePlane(glm::vec3 translate);
	void Init();
	glm::vec3 TranslatePlane;
	

private:
	Model PlaneModel;
};

