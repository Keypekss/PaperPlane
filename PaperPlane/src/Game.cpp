#include "Game.h"
#include "SpriteRenderer.h"
#include "stb_image.h"

SpriteRenderer* LineRenderer;
SpriteRenderer* BlockRenderer;
Shader modelShader;
Shader silhouetteShader;
Shader skyboxShader;

void Game::Init()
{
	Shader lineShader("Shaders/line.vert", "Shaders/line.frag");
	LineRenderer = new SpriteRenderer(lineShader);

	Shader blockShader("Shaders/block.vert", "Shaders/block.frag");
	BlockRenderer = new SpriteRenderer(blockShader);

	modelShader = Shader("Shaders/modelShader.vert", "Shaders/modelShader.frag");
	silhouetteShader = Shader("Shaders/silhouette.vert", "Shaders/silhouette.frag");
	skyboxShader = Shader("Shaders/skybox.vert", "Shaders/skybox.frag");

	plane = Plane();
	plane.Init();

	InitSkybox();
}

unsigned int skyboxVAO = 0, skyboxVBO = 0, cubemapTexture = 0;
void Game::InitSkybox()
{
	// initialize if necessary
	if (skyboxVAO == 0) {
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		// load skybox texture
		std::vector<std::string> faces{
				"Resources/skybox/right.jpg",
				"Resources/skybox/left.jpg",
				"Resources/skybox/top.jpg",
				"Resources/skybox/bottom.jpg",
				"Resources/skybox/front.jpg",
				"Resources/skybox/back.jpg"
		};
		cubemapTexture = loadCubemap(faces);

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		skyboxShader.use();
		skyboxShader.setInt("skybox", 1);
	}
}

void Game::DrawSkybox(Camera& camera)
{	
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

unsigned int Game::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (size_t i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		} else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

bool FirstRoomInserted = false;
void Game::GenerateRooms(Camera &camera)
{	
	if (!FirstRoomInserted) {
		Room room{ glm::vec3(0.0f) };
		room.GenerateRoom(18);
		Rooms.push_back(room);
		FirstRoomInserted = true;
	}

	while (Rooms.size() != RoomCount) {
		// construct a room based on the position of the last room in the vector
		Room room{ glm::vec3(0.0f, 0.0f, (*Rooms.rbegin()).GetPos().z - (*Rooms.rbegin()).GetDepth()) };
		room.GenerateRoom(18);
		Rooms.push_back(room);
	}

	for (auto & Room : Rooms) {
		Room.DrawDebugCube(*LineRenderer, *BlockRenderer ,camera);
	}
}

void Game::RemoveRoom(Camera &camera)
{
	// remove the room if its behind the camera
	if (camera.Position.z < Rooms.at(0).GetPos().z - Rooms.at(0).GetDepth()) {
		auto first_it = Rooms.begin();
		Rooms.erase(first_it);
	}
}

void Game::Render(Camera &camera)
{
	GenerateRooms(camera);
	RemoveRoom(camera);
	plane.drawPlane(modelShader, camera);
	plane.drawSilhouette(silhouetteShader, camera);
	plane.drawCollisionBox(*LineRenderer, camera);
}

void Game::ProcessInput(float deltaTime, Camera& camera)
{
	// move plane and camera forward
	plane.PlanePos.y -= 5.0f * deltaTime;
	plane.CBoxPos.z -= 5.0f * deltaTime;
	camera.Position.z -= 5.0f * deltaTime; 

	// plane controls
	if (Keys[GLFW_KEY_UP] && !KeysProcessed[GLFW_KEY_UP])
	{
		// keeps plane in the bounding box
		if (plane.PlanePos.z + 3.0f < plane.InitialPlanePos.z + 6.0f) {
			plane.PlanePos.z += 3.0f;
			plane.CBoxPos.y += 3.0f;
			camera.Position.y += 3.0f;
			KeysProcessed[GLFW_KEY_UP] = true;
		}
	}
	if (Keys[GLFW_KEY_DOWN] && !KeysProcessed[GLFW_KEY_DOWN])
	{
		if (plane.PlanePos.z - 3.0f > plane.InitialPlanePos.z - 6.0f) {
			plane.PlanePos.z -= 3.0f;
			plane.CBoxPos.y -= 3.0f;
			camera.Position.y -= 3.0f;
			KeysProcessed[GLFW_KEY_DOWN] = true;
		}
	}
	if (Keys[GLFW_KEY_LEFT] && !KeysProcessed[GLFW_KEY_LEFT])
	{
		if (plane.PlanePos.x + 3.0f < plane.InitialPlanePos.x + 6.0f) {
			plane.PlanePos.x += 3.0f;
			plane.CBoxPos.x -= 3.0f;
			camera.Position.x -= 3.0f;
			KeysProcessed[GLFW_KEY_LEFT] = true;
		}
	}
	if (Keys[GLFW_KEY_RIGHT] && !KeysProcessed[GLFW_KEY_RIGHT])
	{
		if (plane.PlanePos.x - 3.0f > plane.InitialPlanePos.x - 6.0f) {
			plane.PlanePos.x -= 3.0f;
			plane.CBoxPos.x += 3.0f;
			camera.Position.x += 3.0f;
			KeysProcessed[GLFW_KEY_RIGHT] = true;
		}
	}
}

void Game::Update(float deltaTime, Camera& camera)
{
	Render(camera);
	DrawSkybox(camera);
	ProcessInput(deltaTime, camera);
	DoCollisions();
}

bool Game::CheckCollision(Plane& plane, Block& block)
{
	// Note: plane collision box position is bottom left side whereas 
	// block's position is middle of the block so we do addition to plane pos to make it in the middle as well

	// collision x-axis?
	bool collisionX = plane.CBoxPos.x + block.BWidth + plane.CBoxWidth >= block.GetPos().x &&
		block.GetPos().x + block.BWidth >= plane.CBoxPos.x;
	// collision y-axis?
	bool collisionY = plane.CBoxPos.y + + block.BHeight + plane.CBoxHeight >= block.GetPos().y &&
		block.GetPos().y + block.BHeight >= plane.CBoxPos.y;	

	// collision z-axis?
	bool collisionZ = plane.CBoxPos.z - plane.CBoxDepth <= block.GetPos().z &&
		block.GetPos().z - block.BDepth <= plane.CBoxPos.z;

	// collision only if on both axes
	return collisionX && collisionY && collisionZ;
}

void Game::DoCollisions()
{	
	// check collisions with the blocks only in the first room
	for (Block& block : Rooms.at(0).GetBlocks()) {		
		if (CheckCollision(plane, block)) 
			std::cout << "Plane collided." << std::endl;
	}
	std::cout << std::endl;
}

Game::~Game()
{
	delete LineRenderer;
	delete BlockRenderer;
}

void Game::Clear()
{
}

