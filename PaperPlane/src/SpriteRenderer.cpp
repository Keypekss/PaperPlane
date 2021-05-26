
#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
}

void SpriteRenderer::DrawLine(glm::vec3 location, glm::vec3 startPos, glm::vec3 endPos, glm::vec3 color, Camera &camera)
{
	this->shader.use();

	this->shader.setVec3("lineColor", color);

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	this->shader.setMat4("projection", projection);
	
	glm::mat4 view = camera.GetViewMatrix();
	this->shader.setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, location);
	this->shader.setMat4("model", model);

	unsigned int lineVAO, lineVBO;
	int vertices[] = {
		startPos.x, startPos.y, startPos.z,
		endPos.x, endPos.y, endPos.z
	};

	glGenVertexArrays(1, &lineVBO);
	glGenBuffers(1, &lineVAO);

	glBindVertexArray(lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);	

	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &lineVAO);
	glDeleteBuffers(1, &lineVBO);
}

void SpriteRenderer::DrawBlock(glm::vec3 pos, float width, float height, float depth, glm::vec3 color, Camera& camera)
{
	this->shader.use();

	this->shader.setVec3("blockColor", color);

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	this->shader.setMat4("projection", projection);

	glm::mat4 view = camera.GetViewMatrix();
	this->shader.setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	this->shader.setMat4("model", model);

	unsigned int blockVBO, blockVAO;
	float blockVertices[] = {
		// Back face
		-width, -height, -depth,  // Bottom-left
		 width,  height, -depth,  // top-right
		 width, -height, -depth,  // bottom-right         
		 width,  height, -depth,  // top-right
		-width, -height, -depth,  // bottom-left
		-width,  height, -depth,  // top-left
		// Front face
		-width, -height,  depth,  // bottom-left
		 width, -height,  depth,  // bottom-right
		 width,  height,  depth,  // top-right
		 width,  height,  depth,  // top-right
		-width,  height,  depth,  // top-left
		-width, -height,  depth,  // bottom-left
		// Left face
		-width,  height,  depth,  // top-right
		-width,  height, -depth,  // top-left
		-width, -height, -depth,  // bottom-left
		-width, -height, -depth,  // bottom-left
		-width, -height,  depth,  // bottom-right
		-width,  height,  depth,  // top-right
		// Right face
		 width,  height,  depth,  // top-left
		 width, -height, -depth,  // bottom-right
		 width,  height, -depth,  // top-right         
		 width, -height, -depth,  // bottom-right
		 width,  height,  depth,  // top-left
		 width, -height,  depth,  // bottom-left     
		// Bottom face
		-width, -height, -depth,  // top-right
		 width, -height, -depth,  // top-left
		 width, -height,  depth,  // bottom-left
		 width, -height,  depth,  // bottom-left
		-width, -height,  depth,  // bottom-right
		-width, -height, -depth,  // top-right
		// Top face
		-width,  height, -depth,  // top-left
		 width,  height,  depth,  // bottom-right
		 width,  height, -depth,  // top-right     
		 width,  height,  depth,  // bottom-right
		-width,  height, -depth,  // top-left
		-width,  height,  depth   // bottom-left        
	};

	//std::cout << "block: " << sizeof(blockVertices) << std::endl;

	glGenVertexArrays(1, &blockVAO);
	glGenBuffers(1, &blockVBO);

	glBindVertexArray(blockVAO);
	glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blockVertices), blockVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &blockVAO);
	glDeleteBuffers(1, &blockVBO);
}
