#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Plane.h"

Plane::Plane() :
	TranslatePlane(glm::vec3(1.0f)), PlaneModel(glm::vec3(0.0f), glm::vec3(1.0f), false)
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
	model = glm::translate(model, glm::vec3(-4.5f, 9.0f, 3.6f));
	model = glm::translate(model, TranslatePlane);
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
	model = glm::translate(model, glm::vec3(-4.5f, 9.0f, 3.6f));
	model = glm::translate(model, TranslatePlane);
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

void Plane::setTranslatePlane(glm::vec3 translate)
{
	TranslatePlane = translate;
}

void Plane::Init()
{
	PlaneModel.loadModel("Resources/Models/plane_model/scene.gltf");
}
