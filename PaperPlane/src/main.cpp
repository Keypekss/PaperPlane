#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"

#include "Shader.h"
#include "Camera.h"
#include "Game.h"
#include "Plane.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, float dt);
unsigned int loadCubemap(std::vector<std::string> faces);
void drawSkybox(Shader& skyboxShader);

const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;

// camera
Camera camera = Camera(glm::vec3(4.2f, 4.7f, 13.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool enableCameraMovement = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//keys
bool Keys[1024];

// ImGUI state
// ----------------------------------------------	
ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
// paperPlane
glm::vec3	PRotAxis	= glm::vec3(1.0f, 0.0f, 0.0f);
float		PRotAngle	= 180;
glm::vec3	PTranslate	= glm::vec3(1.0f);
// camera
float		CRotYaw		= 0.0f;
float		CRotPitch	= 0.0f;
glm::vec3	CTranslate	= glm::vec3(0.0f);

// game
Game planeGame = Game();

int main()
{
	//initialize glfw and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window creation
	GLFWwindow* mainWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLDemo", nullptr, nullptr);
	if (mainWindow == nullptr) {
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mainWindow);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(mainWindow, mouse_callback);
	glfwSetKeyCallback(mainWindow, key_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	// initialize ImGUI
// 	IMGUI_CHECKVERSION();
// 	ImGui::CreateContext();
// 	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
// 	ImGui::StyleColorsDark();
// 	ImGui_ImplOpenGL3_Init((char*)glGetString(330));

	// load shaders
	Shader skyboxShader("Shaders/skybox.vert", "Shaders/skybox.frag");

	// global openGL state
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	
	planeGame.Init();

	//render loop
	while (!glfwWindowShouldClose(mainWindow)) {

		// start ImGUI
// 		ImGui_ImplOpenGL3_NewFrame();
// 		ImGui_ImplGlfw_NewFrame();
// 		ImGui::NewFrame();

		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(mainWindow, deltaTime);		

		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		
		drawSkybox(skyboxShader);
		planeGame.Update(deltaTime, camera);
// 		paperPlaneGame.GenerateRooms(camera);
// 		paperPlaneGame.RemoveRoom(camera);
// 		drawPaperPlane(modelShader);
// 		drawSilhouette(silhouetteShader);


		// ImGui
		// ----------------------------------------------	
// 		{
// 			ImGui::Begin("Transformations");
// 						
// 			// translate camera
// 			ImGui::SliderFloat3("CTranslate ", glm::value_ptr(CTranslate), -25.0f, 25.0f);
// 			ImGui::SliderFloat("CRotYaw ", &CRotYaw, -90.0f, 90.0f);
// 			ImGui::SliderFloat("CRotPitch ", &CRotPitch, -90.0f, 90.0f);
// 
// 			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
// 			ImGui::End();
// 		}
// 
// 		ImGui::Render();
// 		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	// shutdown ImGUI
// 	ImGui_ImplOpenGL3_Shutdown();
// 	ImGui_ImplGlfw_Shutdown();
// 	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float dt)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// camera free control for debugging
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (enableCameraMovement) {
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

// glfw: whenever a button is pressed, this callback is called
// ----------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS && enableCameraMovement) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		enableCameraMovement = false;
	} else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS && !enableCameraMovement) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		enableCameraMovement = true;
	}
	
	// update camera
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		camera.Position = CTranslate;
		camera.ProcessMouseMovement(CRotYaw, CRotPitch);
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) 
		planeGame.Keys[GLFW_KEY_UP] = true;
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		planeGame.Keys[GLFW_KEY_UP] = false;

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) 
		planeGame.Keys[GLFW_KEY_DOWN] = true;
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		planeGame.Keys[GLFW_KEY_DOWN] = false;
	
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) 
		planeGame.Keys[GLFW_KEY_LEFT] = true;
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		planeGame.Keys[GLFW_KEY_LEFT] = false;
	
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) 
		planeGame.Keys[GLFW_KEY_RIGHT] = true;
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		planeGame.Keys[GLFW_KEY_RIGHT] = false;
	
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadCubemap(std::vector<std::string> faces)
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

unsigned int skyboxVAO = 0, skyboxVBO = 0, cubemapTexture = 0;
void drawSkybox(Shader &skyboxShader)
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

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
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
