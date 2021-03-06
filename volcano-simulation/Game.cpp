#include "Game.h"

//Private functions
void Game::initGLFW()
{
	//INIT GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initWindow(
	const char* title,
	bool resizable	
)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MAC OS

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback);
	//IMPORTANT WHITH PERSPECTIVE MATRIX!!!

	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(this->window); //IMPORTANT!!
}

void Game::initGLEW()
{
	//INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	//Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void Game::initShaders()
{
	std::string vertex_core = "vertex_core.glsl";
	std::string fragment_core = "fragment_core.glsl";
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,vertex_core.c_str(),fragment_core.c_str() ));
}

void Game::initTextures()
{
	//TEXTURE 0
	this->textures.push_back(new Texture("Images/grid.png", GL_TEXTURE_2D));
	//TEXTURE 1
	this->textures.push_back(new Texture("Images/volcan.png", GL_TEXTURE_2D));
	//TEXTURE 2
	this->textures.push_back(new Texture("Images/lava4.jpg", GL_TEXTURE_2D));
	//TEXTURE 3
	this->textures.push_back(new Texture("Images/ceniza.png", GL_TEXTURE_2D));

}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(5.f), glm::vec3(1.f), glm::vec3(0.f),0, 1));
}

void Game::initOBJModels()
{

}

void Game::initModels()
{
	//piso
	this->models.push_back(new Model(
		glm::vec3(0.f, 0.f, 0.f),
		materials[0],
		textures[TEX_VOLCANO],
		textures[TEX_VOLCANO],
		"OBJFiles/volcan40.obj",
		true
	)
	);

	Volcan = this->models[this->models.size() - 1];
	
}

void Game::initPointLights()
{
	this->pointLights.push_back(new PointLight(glm::vec3(0.f,30.f,0.f)));
}

void Game::initLights()
{
	this->initPointLights();
}

void Game::initUniforms()
{
	//INIT UNIFORMS
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	for (PointLight * pl : this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
}

void Game::updateUniforms()
{
	//Update view matrix (camera)
	this->ViewMatrix = this->camera.getViewMatrix();

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

	for (PointLight * pl : this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}

	//Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

//Constructors / Destructors
Game::Game(
	const char* title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable
)
	:
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR),
	GL_VERSION_MINOR(GL_VERSION_MINOR),
	//camera(glm::vec3(-50.f, 25.f, 30.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
	camera(glm::vec3(0.f, 20.f, 50.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
	//Init variables
	this->aplicarFuerzaEnEsfera = false;
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->camPosition = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->world.setFuerza(glm::vec3(0.f, -0.05f, 0.f), GRAVEDAD);
	this->world.setFuerza(glm::vec3(0.f, 0.f, 0.f), VELOCIDAD);
	this->world.setFuerza(glm::vec3(0.f, 0.f, 0.f), FRICCION);
	this->world.setFuerza(glm::vec3(0.f, 0.f, 0.f), REPULSION);

	this->nroLava = 30;

	this->nroLava = 7000;
	glm::vec3 v1(-25.f, 0.f, 25.f);
	glm::vec3 v2(25.f, 0.f, 25.f);
	glm::vec3 v3(-25.f, 0.f, -25.f);
	glm::vec3 v4(25.f, 0.f, -25.f);
	glm::vec3 v1v2(v2 - v1);
	glm::vec3 v1v3(v1 - v3);
	glm::vec3 v2v4(v4 - v2);
	glm::vec3 v3v4(v3 - v4);

	float totalPSO = nroLava / 4;

	for (float i = 0; i < totalPSO; i++) {
		lava.push_back(new PSO(v1 + (v1v2 / totalPSO) * i, 1, 0.1, 2, 0.6, 0.02f));
		lava.push_back(new PSO(v2 + (v2v4 / totalPSO) * i, 1, 0.1, 2, 0.6, 0.02f));
		lava.push_back(new PSO(v4 + (v3v4 / totalPSO) * i, 1, 0.1, 2, 0.6, 0.02f));
		lava.push_back(new PSO(v3 + (v1v3 / totalPSO) * i, 1, 0.1, 2, 0.6, 0.02f));
	}

	 this->nroCeniza = 1000;
	 float ladosCeniza = nroCeniza / 4;

	 float rand_1;
	 float rand_2;
	 float rand_3;
	 float rand_4;

	 for (float i = 0; i < ladosCeniza ; i++) {
		rand_1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		rand_2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		rand_3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		rand_4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		 ceniza.push_back(new PSO((v1 + (v1v2 / ladosCeniza) * i)*rand_1, 1, 0.1, 1.1, 1.1, 0.5f, glm::vec3(0.f, -0.1f, 0.f)));
		 ceniza.push_back(new PSO((v2 + (v2v4 / ladosCeniza) * i)*rand_2, 1, 0.1, 1.1, 1.1, 0.5f, glm::vec3(0.f, -0.1f, 0.f)));
		 ceniza.push_back(new PSO((v4 + (v3v4 / ladosCeniza) * i)*rand_3, 1, 0.1, 1.1, 1.1, 0.5f, glm::vec3(0.f, -0.1f, 0.f)));
		 ceniza.push_back(new PSO((v3 + (v1v3 / ladosCeniza) * i)*rand_4, 1, 0.1, 1.1, 1.1, 0.5f, glm::vec3(0.f, -0.1f, 0.f)));
	 }
		




	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();

	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initOBJModels();
	this->initModels();
	this->initLights();
	this->initUniforms();
	
	for (int i = 0; i < lava.size(); i++) {
		lava[i]->init(&models, materials[0], textures[TEX_LAVA],glm::vec3(0,25.f,0));
	}
	for (int i = 0; i < ceniza.size(); i++) {
		ceniza[i]->init(&models, materials[0], textures[TEX_CENIZA], glm::vec3(0, 30.f, 0));
	}
	
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (int i = 0; i < lava.size(); i++) {
		delete lava[i];
	}


	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (auto*& i : this->models)
		delete i;

	for (size_t i = 0; i < this->pointLights.size(); i++)
		delete this->pointLights[i];
	
}

//Accessor
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Modifier	
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

//Functions
void Game::updateDt()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	//Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	//Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;

}

void Game::updateKeyboardInput()
{
	//Program
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}

	if (glfwGetKey(this->window, GLFW_KEY_Z) == GLFW_PRESS) {
		aplicarFuerzaEnEsfera = true;
	}

	//Camera
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->camera.move(this->dt, FORWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->camera.move(this->dt, BACKWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->camera.move(this->dt, LEFT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->camera.move(this->dt, RIGHT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->camPosition.y -= 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->camPosition.y += 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_KP_5) == GLFW_PRESS)
	{
		glm::vec3 Up(0.f, 0.5f, 0.f);
		Particle->darFuerzas(Up);

	}
	if (glfwGetKey(this->window, GLFW_KEY_KP_4) == GLFW_PRESS)
	{
		glm::vec3 Left(-0.5f, 0.0f, 0.f);
		Particle->darFuerzas(Left);
	}
	if (glfwGetKey(this->window, GLFW_KEY_KP_6) == GLFW_PRESS)
	{
		glm::vec3 Right(0.5f, 0.f, 0.f);
		Particle->darFuerzas(Right);
	}
	if (glfwGetKey(this->window, GLFW_KEY_KP_8) == GLFW_PRESS)
	{
		glm::vec3 Down(0.f, 0.f, -0.5f);
		Particle->darFuerzas(Down);
	}
	if (glfwGetKey(this->window, GLFW_KEY_KP_2) == GLFW_PRESS)
	{
		glm::vec3 Down(0.f, 0.f, 0.5f);
		Particle->darFuerzas(Down);
	}
}

void Game::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::update()
{
	//UPDATE INPUT ---

	
	this->updateDt();
	this->updateInput();
	

	if (aplicarFuerzaEnEsfera) {
		this->world.aplicarFuerza(lava);
		this->world.aplicarFuerza(ceniza);
	}

	for (int i = 0;i<lava.size();i++) {
		this->world.checkColision(aplicarFuerzaEnEsfera, Volcan, lava[i]);
	}
	


}

void Game::render()
{
	//UPDATE --- 
	//updateInput(window);

	//DRAW ---
	//Clear
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
	//Update the uniforms
	this->updateUniforms();

	//Render models
	for (auto& i : this->models)
		i->render(this->shaders[SHADER_CORE_PROGRAM]);

	//End Draw
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Static functions
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
};
