#include "Coin.h"
#include "Model.h"

void Coin::Init()
{
	// load coin texture
	CoinModel = Model();
	CoinModel.loadModel("Resources/Models/coin/scene.gltf");
}

void Coin::DrawCoin(Shader coinShader, Camera& camera, float deltaTime, unsigned int enumPos)
{
	coinShader.use();

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
	coinShader.setMat4("view", view);
	coinShader.setMat4("projection", projection);

	glm::mat4 model = glm::mat4(1.0f);

	struct CoinPos {
		glm::vec3 BOTTOM_LEFT	= glm::vec3(1.5f, 1.5f, 0.0f);
		glm::vec3 BOTTOM_MID	= glm::vec3(4.5f, 1.5f, 0.0f);
		glm::vec3 BOTTOM_RIGHT	= glm::vec3(7.5f, 1.5f, 0.0f);
		glm::vec3 CENTER_LEFT	= glm::vec3(1.5f, 4.5f, 0.0f);
		glm::vec3 CENTER		= glm::vec3(4.5f, 4.5f, 0.0f);
		glm::vec3 CENTER_RIGHT	= glm::vec3(7.5f, 4.5f, 0.0f);
		glm::vec3 TOP_LEFT		= glm::vec3(1.5f, 7.5f, 0.0f);
		glm::vec3 TOP_MID		= glm::vec3(4.5f, 7.5f, 0.0f);
		glm::vec3 TOP_RIGHT		= glm::vec3(7.5f, 7.5f, 0.0f);
	} pos;

	glm::vec3 coinPos;
	switch (enumPos) {
	case 0:
		coinPos = pos.BOTTOM_LEFT;
		break;
	case 1:
		coinPos = pos.BOTTOM_MID;
		break;
	case 2:
		coinPos = pos.BOTTOM_RIGHT;
		break;
	case 3:
		coinPos = pos.CENTER_LEFT;
		break;
	case 4:
		coinPos = pos.CENTER;
		break;
	case 5:
		coinPos = pos.CENTER_RIGHT;
		break;
	case 6:
		coinPos = pos.TOP_LEFT;
		break;
	case 7:
		coinPos = pos.TOP_MID;
		break;
	case 8:
		coinPos = pos.TOP_RIGHT;
		break;
	default:	
		coinPos = glm::vec3(0.0f);
		std::cout << "Error: Shouldn't reach here" << std::endl;
		break;
	}
	
	model = glm::translate(model, coinPos);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	angularSpeed += 90.0f * deltaTime;
	model = glm::rotate(model, glm::radians(angularSpeed), glm::vec3(0.0, 0.0f, 1.0f));

	coinShader.setMat4("model", model);

	CoinModel.render(coinShader, false);
}
