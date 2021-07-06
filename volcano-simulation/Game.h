#pragma once

#include "libs.h"
#include "Camera.h"

enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_GRID = 0, TEX_VOLCANO, TEX_LAVA,TEX_CENIZA};
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MESH_QUAD = 0 };

class Game
{
private:
	//Variables
		//Window
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	//OpenGL Context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	//Delta time
	float dt;
	float curTime;
	float lastTime;

	//Mouse Input
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	//Camera
	Camera camera;

	//Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;

	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	//Shaders
	std::vector<Shader*> shaders;

	//Textures
	std::vector<Texture*> textures;

	//Materials
	std::vector<Material*> materials;

	//Models
	std::vector<Model*> models;
	Model* Particle;

	//Lights
	std::vector<PointLight*> pointLights;

	//Private functions
	void initGLFW();
	void initWindow(
		const char* title,
		bool resizable
	);
	void initGLEW(); //AFTER CONTEXT CREATION!!!
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initOBJModels();
	void initModels();
	void initPointLights();
	void initLights();
	void initUniforms();
	void updateUniforms();

	//Static variables
	int nroLava;
	std::vector<PSO*> lava;

	int nroCeniza;
	std::vector<PSO*> ceniza;
	//Physics
	Physics world;

	//Variables
	bool aplicarFuerzaEnEsfera = true;

	Model* Volcan;

public:
	//Constructors / Destructors
	Game(
		const char* title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
		bool resizable
	);
	virtual ~Game();

	//Accessors
	int getWindowShouldClose();

	//Modifiers
	void setWindowShouldClose();

	//Functions
	void updateDt();
	void updateMouseInput();
	void updateKeyboardInput();
	void updateInput();
	void update();
	void render();

	//Static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
	bool myfunction(glm::vec3 i, glm::vec3 j) { return (i.y < j.y); }

};