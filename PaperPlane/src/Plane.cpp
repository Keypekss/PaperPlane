#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Plane.h"

Plane::Plane() :
	PlaneModel(glm::vec3(0.0f), glm::vec3(1.0f), false), PlanePos(-3.5f, 9.0f, 4.7f), InitialPlanePos(PlanePos), CBoxPos(3.7f, 4.3f, 9.0f)
{	
}

void Plane::drawPlane(Shader &modelShader, Camera& camera)
{	
	modelShader.use();

	// set model uniforms			
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1280.0f / 720.0f, 0.1f, 90.0f);
	glm::mat4 view = camera.GetViewMatrix();
	modelShader.setMat4("projection", projection);
	modelShader.setMat4("view", view);
	// draw paperPlane as normal, writing to the stencil buffer
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, PlanePos);
	model = glm::scale(model, glm::vec3(0.01f));
	
	modelShader.setMat4("model", model);

	PlaneModel.render(modelShader, false);
}

void Plane::drawSilhouette(Shader& silhouetteShader, Camera& camera)
{
	// set uniforms
	silhouetteShader.use();

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1280.0f / 720.0f, 0.1f, 90.0f);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, PlanePos);
	model = glm::scale(model, glm::vec3(0.01f));

	silhouetteShader.setMat4("model", model);
	silhouetteShader.setMat4("view", view);
	silhouetteShader.setMat4("projection", projection);

	// draw slightly scaled versions of the plane, this time disabling stencil writing.
	// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
	// the objects' size differences, making it look like borders.
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	PlaneModel.render(silhouetteShader, false);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
}

void Plane::drawCollisionBox(SpriteRenderer& lineRenderer, Camera& camera)
{
	std::vector<std::vector<glm::vec3>> vertices{
		// first point										// second point
		{glm::vec3(0,			0,			0),				glm::vec3(CBoxWidth,	0,			0)}, // front
		{glm::vec3(0,			0,			0),				glm::vec3(0,			CBoxHeight,	0)},
		{glm::vec3(0,			CBoxHeight,	0),				glm::vec3(CBoxWidth,	CBoxHeight,	0)},
		{glm::vec3(CBoxWidth,	CBoxHeight,	0),				glm::vec3(CBoxWidth,	0,			0)},

		{glm::vec3(0,			0,			0),				glm::vec3(0,			0,			-CBoxDepth)}, // sides
		{glm::vec3(0,			CBoxHeight,	0),				glm::vec3(0,			CBoxHeight,	-CBoxDepth)},
		{glm::vec3(CBoxWidth,	CBoxHeight,	0),				glm::vec3(CBoxWidth,	CBoxHeight,	-CBoxDepth)},
		{glm::vec3(CBoxWidth,	0,			0),				glm::vec3(CBoxWidth,	0,			-CBoxDepth)},

		{glm::vec3(0,			0,			-CBoxDepth),	glm::vec3(CBoxWidth,	0,			-CBoxDepth)}, // back
		{glm::vec3(0,			0,			-CBoxDepth),	glm::vec3(0,			CBoxHeight,	-CBoxDepth)},
		{glm::vec3(0,			CBoxHeight,	-CBoxDepth),	glm::vec3(CBoxWidth,	CBoxHeight,	-CBoxDepth)},
		{glm::vec3(CBoxWidth,	CBoxHeight,	-CBoxDepth),	glm::vec3(CBoxWidth,	0,			-CBoxDepth)}
	};

	for (size_t i = 0; i < vertices.size(); i++) {
		glm::vec3 startPos = vertices.at(i).at(0);
		glm::vec3 endPos = vertices.at(i).at(1);
		lineRenderer.DrawLine(CBoxPos, startPos, endPos, glm::vec3(1.0f, 0.0f, 1.0f), camera);
	}
}

void Plane::Init()
{
	PlaneModel.loadModel("Resources/Models/plane_model/scene.gltf");
}
