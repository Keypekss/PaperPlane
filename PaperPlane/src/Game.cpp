#include <iomanip>

#include "Game.h"
#include "SpriteRenderer.h"
#include "stb_image.h"
#include "TextRenderer.h"

SpriteRenderer* LineRenderer;
SpriteRenderer* BlockRenderer;
TextRenderer*	Text;

Shader modelShader;
Shader coinShader;
Shader silhouetteShader;
Shader skyboxShader;
Shader postprocessingShader;

void Game::Init()
{
	Shader lineShader("Shaders/line.vert", "Shaders/line.frag");
	LineRenderer = new SpriteRenderer(lineShader);

	Shader blockShader("Shaders/block.vert", "Shaders/block.frag");
	BlockRenderer = new SpriteRenderer(blockShader);

	modelShader			= Shader("Shaders/modelShader.vert", "Shaders/modelShader.frag");
	coinShader			= Shader("Shaders/modelShader.vert", "Shaders/modelShader.frag");
	silhouetteShader	= Shader("Shaders/silhouette.vert", "Shaders/silhouette.frag");	
	skyboxShader		= Shader("Shaders/skybox.vert", "Shaders/skybox.frag");
	postprocessingShader= Shader("Shaders/postprocessing.vert", "Shaders/postprocessing.frag");

	// initialize custom framebuffer for grayscale rendering
	InitFramebuffer(); 

	// load models and set shader matrices
	plane = Plane();
	plane.Init();

	// initialize coin
	coin = Coin();
	coin.Init();

	// initialize skybox
	InitSkybox();

	// initialize text
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("Resources/fonts/OCRAEXT.TTF", 24);

	// initial state
	State = MENU;	
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


void Game::InitFramebuffer()
{
	if (postprocessingVAO == 0) {
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		// screen quad VAO		
		glGenVertexArrays(1, &postprocessingVAO);
		glGenBuffers(1, &postprocessingVBO);
		glBindVertexArray(postprocessingVAO);
		glBindBuffer(GL_ARRAY_BUFFER, postprocessingVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		postprocessingShader.use();
		postprocessingShader.setInt("screenTexture", 0);		

		// framebuffer configuration		
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture		
		glGenTextures(1, &textureColorbuffer);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Error::Framebuffer: Framebuffer is not complete." << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		room.GenerateRoom(15);
		Rooms.push_back(room);
		FirstRoomInserted = true;
	}

	while (Rooms.size() != RoomCount) {
		// construct a room based on the position of the last room in the vector
		Room room{ glm::vec3(0.0f, 0.0f, (*Rooms.rbegin()).GetPos().z - (*Rooms.rbegin()).GetDepth()) };
		room.GenerateRoom(15);
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

void Game::Render(float deltaTime, Camera &camera)
{
	GenerateRooms(camera);
	RemoveRoom(camera);
	plane.drawPlane(modelShader, camera);
	plane.drawSilhouette(silhouetteShader, camera);
	//plane.drawCollisionBox(*LineRenderer, camera);
	for (auto& room : Rooms) {
		for (auto& coin : room.Coins) {
			coin.DrawCoin(coinShader, camera, deltaTime, angularSpeed);
			//coin.DrawCBox(*LineRenderer, camera);
		}
	}	
	DrawSkybox(camera);	
}

void Game::RenderMenu()
{	
	Text->RenderText("CONTROLS:",							this->Width / 2 - 450, this->Height / 2.0f - 190.0f, 1.0f);
	Text->RenderText("USE ARROW KEYS TO MOVE",				this->Width / 2 - 450, this->Height / 2.0f - 170.0f, 1.0f);
	Text->RenderText("CTRL TO TAKE CONTROL OF MOUSE",		this->Width / 2 - 450, this->Height / 2.0f - 150.0f, 1.0f);
	Text->RenderText("WASD TO MOVE CAMERA INDEPENDENTLY",	this->Width / 2 - 450, this->Height / 2.0f - 130.0f, 1.0f);
	Text->RenderText("ESC to exit",							this->Width / 2 - 450, this->Height / 2.0f - 110.0f, 1.0f);
	Text->RenderText("PRESS ENTER TO START THE GAME",		this->Width / 2 - 450, this->Height / 2.0f - 90.0f, 1.0f);
}

void Game::RenderUI()
{	
	std::stringstream ss;
	ss << std::fixed;
	ss.precision(0);
	ss << this->Score;
	Text->RenderText("SCORE: " + ss.str(), this->Width / 2 - 50, this->Height / 2.0f - 300.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	if (State == GAME_OVER) {
		Text->RenderText("Game Over", this->Width / 2 - 300, this->Height / 2.0f - 130.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

bool flag1 = false;
bool flag2 = false;
bool flag3 = false;
bool flag4 = false;
void Game::ProcessInput(float deltaTime, Camera& camera)
{
	// move plane and camera forward
	if (State == START) {
		plane.PlanePos.y -= 5.0f * deltaTime;
		plane.CBoxPos.z -= 5.0f * deltaTime;
		camera.Position.z -= 5.0f * deltaTime;
	}
	// plane controls
	{

	if (flag1 || Keys[GLFW_KEY_UP] && !KeysProcessed[GLFW_KEY_UP]) {
		if (plane.PlanePos.z < previousPlanePos.z + 3.0f) { // move plane only by 3
			if (plane.PlanePos.z < plane.InitialPlanePos.z + 3.0f) { // keeps plane in the bounding box
				flag1 = true;
				flag2 = false; // only one flag can be true at a time
				flag3 = false;
				flag4 = false;
				Move(camera, deltaTime, 3.0f, DIR_UP);
			}
		} else {
			flag1 = false;
			previousPlanePos = plane.PlanePos;
		}
		KeysProcessed[GLFW_KEY_UP] = true;
	}
	if (flag2 || Keys[GLFW_KEY_DOWN] && !KeysProcessed[GLFW_KEY_DOWN]) {
		if (plane.PlanePos.z > previousPlanePos.z - 3.0f) {
			if (plane.PlanePos.z > plane.InitialPlanePos.z - 3.0f) {
				flag2 = true;
				flag1 = false;
				flag3 = false;
				flag4 = false;
				Move(camera, deltaTime, 3.0f, DIR_DOWN);
			}
		} else {
			flag2 = false;
			previousPlanePos = plane.PlanePos;
		}
		KeysProcessed[GLFW_KEY_DOWN] = true;
	}
	if (flag3 || Keys[GLFW_KEY_LEFT] && !KeysProcessed[GLFW_KEY_LEFT]) {
		if (plane.PlanePos.x < previousPlanePos.x + 3.0f) {
			if (plane.PlanePos.x < plane.InitialPlanePos.x + 3.0f) {
				flag3 = true;
				flag1 = false;
				flag2 = false;
				flag4 = false;
				Move(camera, deltaTime, 3.0f, DIR_LEFT);
			}
		} else {
			flag3 = false;
			previousPlanePos = plane.PlanePos;
		}
		KeysProcessed[GLFW_KEY_LEFT] = true;
	}
	if (flag4 || Keys[GLFW_KEY_RIGHT] && !KeysProcessed[GLFW_KEY_RIGHT]) {
		if (plane.PlanePos.x > previousPlanePos.x - 3.0f) {
			if (plane.PlanePos.x > plane.InitialPlanePos.x - 3.0f) {
				flag4 = true;
				flag1 = false;
				flag2 = false;
				flag3 = false;
				Move(camera, deltaTime, 3.0f, DIR_RIGHT);
			}
		} else {
			flag4 = false;
			previousPlanePos = plane.PlanePos;
		}
		KeysProcessed[GLFW_KEY_RIGHT] = true;
	}
}

	// change game state
	if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER]) {
		KeysProcessed[GLFW_KEY_ENTER] = true;
		State = START;
	}
}

void Game::Move(Camera& camera, float deltaTime, float moveBy, int dir)
{		
	switch (dir) {
	case DIR_UP:		
		plane.PlanePos.z += 10.0f * deltaTime;
		plane.CBoxPos.y += 10.0f * deltaTime;
		camera.Position.y += 10.0f * deltaTime;		
		break;
	case DIR_DOWN:
		plane.PlanePos.z -= 10.0f * deltaTime;
		plane.CBoxPos.y -= 10.0f * deltaTime;
		camera.Position.y -= 10.0f * deltaTime;		
		break;
	case DIR_LEFT:		
		plane.PlanePos.x += 10.0f * deltaTime;
		plane.CBoxPos.x -= 10.0f * deltaTime;
		camera.Position.x -= 10.0f * deltaTime;		
		break;
	case DIR_RIGHT:				
		plane.PlanePos.x -= 10.0f * deltaTime;
		plane.CBoxPos.x += 10.0f * deltaTime;
		camera.Position.x += 10.0f * deltaTime;		
		break;
	default:
		std::cout << "ERROR :: default switch statement called.";
		break;
	}
}

void Game::Update(float deltaTime, Camera& camera, GLFWwindow* window)
{
	if (State == MENU) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		RenderMenu();
		ProcessInput(deltaTime, camera);
	} else if (State == START) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		angularSpeed += 90.0f * deltaTime;
		Render(deltaTime, camera);
		DoCollisions();
		ProcessInput(deltaTime, camera);
		RenderUI();
		Score += deltaTime;
	} else if (State == GAME_OVER) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // bind custom framebuffer before rendering for vfx
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		angularSpeed += 90.0f * deltaTime;
		Render(deltaTime, camera);		
		RenderUI();

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // bind back to default framebuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		postprocessingShader.use(); // render scene grayscale
		glBindVertexArray(postprocessingVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);	
	}
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

bool Game::CheckCollision(Plane& plane, Coin& coin)
{	
	// collision x-axis?
	bool collisionX = plane.CBoxPos.x + coin.CBoxWidth + plane.CBoxWidth >= coin.GetCoinCBPos().x &&
		coin.GetCoinCBPos().x + coin.CBoxWidth >= plane.CBoxPos.x;
	// collision y-axis?
	bool collisionY = plane.CBoxPos.y + coin.CBoxHeight + plane.CBoxHeight >= coin.GetCoinCBPos().y &&
		coin.GetCoinCBPos().y + coin.CBoxHeight >= plane.CBoxPos.y;

	// collision z-axis?
	bool collisionZ = plane.CBoxPos.z - plane.CBoxDepth <= coin.GetCoinCBPos().z &&
		coin.GetCoinCBPos().z - coin.CBoxDepth <= plane.CBoxPos.z;

	// collision only if on both axes
	return collisionX && collisionY && collisionZ;
}

void Game::DoCollisions()
{	
	// check collisions with the blocks only in the first room
	for (Block& block : Rooms.at(0).GetBlocks()) {		
		if (CheckCollision(plane, block))
			State = GAME_OVER;
	}

	// iterate over coins in the first room 
	for (unsigned int i = 0; i < Rooms.at(0).Coins.size(); ++i) {
		if (CheckCollision(plane, Rooms.at(0).Coins.at(i))) { // collision between the plane and coin in the first room
			Rooms.at(0).Coins.erase(Rooms.at(0).Coins.begin() + i); // destroy coin if collision exists
			Score += 10;
		}
	}
}

Game::~Game()
{
	delete LineRenderer;
	delete BlockRenderer;
	delete Text;
}

void Game::Clear()
{
	glDeleteVertexArrays(1, &postprocessingVAO);
	glDeleteBuffers(1, &postprocessingVBO);
	glDeleteFramebuffers(1, &framebuffer);
}

