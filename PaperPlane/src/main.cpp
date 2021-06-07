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

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window, float dt);

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
	glfwSetCursorPosCallback(mainWindow, mouse_callback);
	glfwSetKeyCallback(mainWindow, key_callback);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	// global openGL state
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	// initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init((char*)glGetString(330));
	
	planeGame.Init();

	//render loop
	while (!glfwWindowShouldClose(mainWindow)) {

		// start ImGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(mainWindow, deltaTime);		

		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		
		planeGame.Update(deltaTime, camera);

		// ImGui
		// ----------------------------------------------	
		{			
			ImGui::Begin("Transformations");   

			// translate mirror
			ImGui::SliderFloat3("PaperPlane ", glm::value_ptr(planeGame.plane.CBoxPos), -10.0f, 10.0f);

			// translate camera
			ImGui::SliderFloat3("CTranslate ", glm::value_ptr(CTranslate), -25.0f, 25.0f);
			ImGui::SliderFloat("CRotYaw ", &CRotYaw, -90.0f, 90.0f);
			ImGui::SliderFloat("CRotPitch ", &CRotPitch, -90.0f, 90.0f);


			ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color			

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	// shutdown ImGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		planeGame.Keys[GLFW_KEY_UP] = true;
	}
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
		planeGame.Keys[GLFW_KEY_UP] = false;
		planeGame.KeysProcessed[GLFW_KEY_UP] = false;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		planeGame.Keys[GLFW_KEY_DOWN] = true;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
		planeGame.Keys[GLFW_KEY_DOWN] = false;
		planeGame.KeysProcessed[GLFW_KEY_DOWN] = false;
	}
	
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		planeGame.Keys[GLFW_KEY_LEFT] = true;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
		planeGame.Keys[GLFW_KEY_LEFT] = false;
		planeGame.KeysProcessed[GLFW_KEY_LEFT] = false;
	}
	
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		planeGame.Keys[GLFW_KEY_RIGHT] = true;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
		planeGame.Keys[GLFW_KEY_RIGHT] = false;
		planeGame.KeysProcessed[GLFW_KEY_RIGHT] = false;
	}
	
}
