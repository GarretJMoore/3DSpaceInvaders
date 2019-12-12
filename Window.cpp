#include "Window.h"
#include "Terrain.h"

int Window::width;
int Window::height;

GLuint Window::SCR_WIDTH = 1280;
GLuint Window::SCR_HEIGHT = 960;

const char* Window::windowTitle = "CSE 167 PA2";

Cube* skyboxCube;

bool firstMouse = true;
float lastX;
float lastY;
float yaw2 = 90.0f;
float pitch2 = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

// Toggles (FOR POINTS)
bool FPS = true;
bool collision = true;

// Toggles (FOR DEBUG)
bool freeCam = false;
bool moveMouse = false;

// Shadow (for points)
bool loadShadows = false;
bool showDepth = false;

int numUFO;
int score = 0;
float ufoSpeedZ = 0.005f;
float ufoSpeedY = 0.005f;
float ufoSpeedX = 0.025f;

float cameraSpeed = 0.05f; 

float blasterSpeed = 0.05f;

float currTime = glfwGetTime();
int makeNewRobot = currTime + 3.0f;
int numGenUFOs;

float currDegree = 0;

// Tree Nodes
vector<Transform *> ufoArmy;
vector<Transform*> blasterArmy;

Transform * ufoArmyRoot;
Transform * ufo; // Root of scene graph to draw each ufo
Transform * ufoT;
Geometry * ufoG;

Transform* blasterArmyRoot;
Transform* blaster;
Transform* blasterT;
Geometry* blasterG;

Geometry * sphere;
Geometry * sphere2;

Terrain* terr; // The procedurally generated terrain

int shadeType = 0;
bool mouseHeld = false; // is the left mouse button being held down
bool showSphere = false;

double FOVdeg = 60.0; // used in field of view
double debugFOV = FOVdeg;
bool shouldCull = false;
bool debugMode = false;

glm::vec3 lastPoint; // The last point before moving the mouse in 3d

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 0); // Camera position.
glm::vec3 Window::center(0, 0, -1.0f); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

glm::mat4 Window::lightView;
glm::mat4 Window::lightProjection;
glm::mat4 Window::lightSpaceMatrix;
glm::vec3 Window::lightPos = glm::vec3(0.0f, 0.0f, 20.0f);

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, eye + Window::center, Window::up); 

GLuint Window::program; // The shader program id.
GLuint Window::program2; // shader program id for everything but skybox

GLuint Window::render; // Shader program id used to render depth map;
GLuint Window::depth; // Shader program used to create the depth map;
GLuint Window::shadow; // Shader program used to render the shadows;

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.

GLuint Window::lightSpaceLoc; // Location of lightSpaceMatrix in depth shader;
GLuint Window::lightPosShadow; // Location of light position in shadow shader;
GLuint Window::viewPosShadow; // Location of view position in shadow shader;
GLuint Window::modelShadow; // Location of model matrix in shadow shader;
GLuint Window::viewShadow; // Location of view matrix in shadow shader;
GLuint Window::projectionShadow; // Location of projection matrix in shadow shader;
GLuint Window::lightSpaceMatShadow; // Location of lghtSpaceMatrix in shadow shader;

GLuint Window::depthMapFBO; // Frame buffer object used to hold texture for depth map
GLuint Window::depthMap; // Map of the depths of the objects in orthrographic
GLuint Window::quadVAO;
GLuint Window::quadVBO;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	program2 = LoadShaders("shaders/shader2.vert", "shaders/shader2.frag");

	render = LoadShaders("shaders/shades/render.vert",
		"shaders/shades/render.frag");
	depth = LoadShaders("shaders/shades/depth.vert",
		"shaders/shades/depth.frag");
	shadow = LoadShaders("shaders/shades/shadowMap.vert",
		"shaders/shades/shadowMap.frag");


	// Check the shader program.
	if (!program2)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program2);

	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program2, "projection");
	viewLoc = glGetUniformLocation(program2, "view");
	modelLoc = glGetUniformLocation(program2, "model");
	colorLoc = glGetUniformLocation(program2, "color");

	glUseProgram(depth);
	lightSpaceLoc = glGetUniformLocation(depth, "lightSpaceMatrix");

	glUseProgram(shadow);
	lightPosShadow = glGetUniformLocation(shadow, "lightPos");
	viewPosShadow = glGetUniformLocation(shadow, "viewPos");
	modelShadow = glGetUniformLocation(shadow, "model");
	viewShadow = glGetUniformLocation(shadow, "view");
	projectionShadow = glGetUniformLocation(shadow, "projection");
	lightSpaceMatShadow = glGetUniformLocation(shadow, "lightSpaceMatrix");

	return true;
}

bool Window::initializeObjects()
{
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float near_plane = 1.0f, far_plane = 1000.0f;
	lightProjection = glm::ortho(-80.0f, 80.0f, -40.0f, 20.0f,
		near_plane, far_plane);
	//glm::vec3 lightPos(0.0f, -5.0f, 20.0f);
	lightView = glm::lookAt(lightPos, center, up);
	lightSpaceMatrix = lightProjection * lightView;


	GLfloat quadVertices[] = {
	   -1.0f,  1.0f,  0.0f, 1.0f,
	   -1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

	   -1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f,
	};
	GLuint fbo_indices[] = {
		// Left bottom triangle
		0, 1, 2,
		// Right top triangle
		2, 3, 0
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
		(void*)(2 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	terr = new Terrain();
	terr->model = glm::translate(terr->model, glm::vec3(0.0f, -20.0f, 0.0f));

	glm::mat4 identity(1.0f);

	glm::mat4 ufoTM = glm::scale(identity, glm::vec3(.9f));

	glm::mat4 ufoStart = glm::translate(identity, glm::vec3(-40, 0, -150));
	ufoArmy.push_back(new Transform(ufoStart));


	glm::mat4 smallIdentity = glm::scale(identity, glm::vec3(.5f));
	ufoArmyRoot = new Transform(smallIdentity);
	
	ufo = new Transform(identity);
	ufoT = new Transform(ufoTM);
	ufoG = new Geometry("ufo.obj", 1, "body_s.obj", false, false);
//////////////////////////////////////////////////////////////////////////////////
	blasterArmyRoot = new Transform(smallIdentity);
	glm::mat4 blastTM = glm::scale(identity, glm::vec3(.1f));

	blaster = new Transform(identity);
	blasterT = new Transform(blastTM);
	blasterG = new Geometry("blaster.obj", 1, "antenna_s.obj", false, false);

	blaster->addChild(blasterT);
	blasterT->addChild(blasterG);
/////////////////////////////////////////////////////////////////////////////////

	sphere = new Geometry("sphere2.obj", 1, "sphere1", true, false);
	sphere2 = new Geometry("sphere2.obj", 1, "sphere2", true, false);

	// Set up scene graph for UFOs
	for (auto currUFO : ufoArmy) {

		currUFO->addChild(ufo);
		currUFO->addChild(sphere); // bounding sphere
		ufoArmyRoot->addChild(currUFO);
		
	}

	ufo->addChild(ufoT);
	ufoT->addChild(ufoG);
	/////////////////////////////////////


	skyboxCube = new Cube(1.0f);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete ufoG;
	delete ufoT;
	delete ufo;

	for (int i = 0; i < ufoArmy.size(); i++) {
		delete ufoArmy[i];
	}

	// Delete the shader program.
	glDeleteProgram(program);
	glDeleteProgram(program2);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);



#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	lastX = width / 2;
	lastY = height / 2;

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix. FIELD OF VIEW
	Window::projection = glm::perspective(glm::radians(FOVdeg), 
		double(width) / (double)height, 1.0, 1000.0);

	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void Window::idleCallback()
{
	
	
}

void Window::displayCallback(GLFWwindow* window)
{	
	view = camera.GetViewMatrix();
	viewLoc = glGetUniformLocation(program2, "view");

	glUseProgram(program2);
	string title = "Current Score: ";
	title = title + std::to_string(score).c_str();
	glfwSetWindowTitle(window, title.c_str()); // Set title to numUFO rendered
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Specify the values of the uniform variables we are going to use.

	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// Set background color
	glClearColor(0.75f, 0.52f, 0.3f, 1.0f); 

	currTime = glfwGetTime();
	
	// Add a robot every 5 seconds to the UFO scene graph
	if (currTime >= makeNewRobot) {

		// Make a new robot and generate it
		glm::mat4 ufoStart = glm::translate(mat4(1.0f), glm::vec3(-40, 0, -200));
		Transform* newUFOT = new Transform(ufoStart);

		newUFOT->addChild(ufo);
		newUFOT->addChild(sphere); // bounding sphere
		newUFOT->currDirection = numGenUFOs % 2;
		newUFOT->prevDirection = 1 + (numGenUFOs % 2);
		ufoArmyRoot->addChild(newUFOT);

		// Set timer back to 0
		makeNewRobot += 1.0f;
		numGenUFOs++;
	}
	
	// Game Over, pause for 10 seconds then close window 
	if (currTime >= 60.0) {
		
		string title = "GAME OVER!!!!!!!! FINAL SCORE: ";
		title = title + std::to_string(score).c_str();
		glfwSetWindowTitle(window, title.c_str()); // Set title to numUFO rendered

		// Sleep for 10 seconds then close
		_sleep(10000);
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Move all the robots in the scene graph a small amount
	int count = 0;
	for (auto currUFO : ufoArmyRoot->children) {
	
		
		// Get the transform and the geometry want to update
		Transform* currTransform = (Transform*)currUFO;

		// Check if we need to change direction
		float changeDirection = currTransform->changeDirection;
		// Get the direction we want to go
		int currDirection = currTransform->currDirection;
	
		// Check if we need to change direction with a basic timer
		if (currTime - changeDirection >= 5.0f) {
			currTransform->changeDirection = currTime;
			currTransform->numChanges++;

			// Need to go right or left now
			
			if (currDirection == 0) {
				// Go right
				if(currTransform->prevDirection == 1) {
					currTransform->currDirection = 2;
				}
				// go left
				else {
					currTransform->currDirection = 1;
				}
				currTransform->prevDirection = 0;
			}

			
			// Go forward now
			else {
				currTransform->currDirection = 0;
				currTransform->prevDirection = currDirection;
			}

			// Update currDirection
			currDirection = currTransform->currDirection;
		}

		glm::mat4 tempM = currTransform->M;
		glm::vec3 currPoint(tempM[3][0], tempM[3][1], tempM[3][2]);
		
		// Calculate where the UFO should move
		// Go forward
		if (currDirection == 0) {

			float newZ = currPoint.z + ufoSpeedZ;
			currPoint.z = newZ;

		}

		// Go right
		else if (currDirection == 1) {

			float newX = currPoint.x + ufoSpeedX;
			currPoint.x = newX;
		}

		// Go left
		else if(currDirection == 2) {

			float newX = currPoint.x - ufoSpeedX;
			currPoint.x = newX;
		}
		
		tempM = glm::translate(glm::mat4(1.0f), currPoint);
		currTransform -> M = tempM;

	}

	// Render the scene graph.
	numUFO = 0; // reset number ufos incase it changes
	
	numUFO = ufoArmyRoot->children.size();
	
	if (loadShadows) {
		shadowMapping();
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ufoArmyRoot->draw(program2, glm::mat4(1.0f));

		processInput(window);




		for (auto currBlaster : blasterArmyRoot->children) {

			Transform* currTransform = (Transform*)currBlaster;

			glm::mat4 tempM = currTransform->M;
			glm::vec3 currPoint(tempM[3][0], tempM[3][1], tempM[3][2]);

			currPoint.z = currPoint.z - blasterSpeed;

			tempM = glm::translate(glm::mat4(1.0f), currPoint);
			currTransform->M = tempM;


		}

		blasterArmyRoot->draw(program2, glm::mat4(1.0f));
	}

	if (!showDepth) {
		// Skybox
		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		skyboxCube->draw2(program);

		glUseProgram(program2);
		GLuint modelLoc = glGetUniformLocation(program2, "model");
		GLuint colorLoc = glGetUniformLocation(program2, "color");

		glUniform3fv(colorLoc, 1, glm::value_ptr(terr->color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(terr->model));
		terr->draw();
	}



	// Collision Detection
	collisionDetection();

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
	
}

void Window::shadowMapping() {
	glUseProgram(depth);
	glUniformMatrix4fv(lightSpaceLoc, 1, GL_FALSE,
		glm::value_ptr(lightSpaceMatrix));
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	glClearColor(0.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Render the scene using the depth shader to get the depth map
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ufoArmyRoot->draw(depth, glm::mat4(1.0f));

	//processInput(window);




	for (auto currBlaster : blasterArmyRoot->children) {

		Transform* currTransform = (Transform*)currBlaster;

		glm::mat4 tempM = currTransform->M;
		glm::vec3 currPoint(tempM[3][0], tempM[3][1], tempM[3][2]);

		currPoint.z = currPoint.z - blasterSpeed;

		tempM = glm::translate(glm::mat4(1.0f), currPoint);
		currTransform->M = tempM;


	}

	blasterArmyRoot->draw(depth, glm::mat4(1.0f));

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	skyboxCube->draw2(program);

	/*
	if (!toCull)
		root->draw(depth, glm::mat4(1.0f));

	else {
		for (int i = 0; i < root->children.size(); i++) {
			if (root->children[i]->toDraw)
				root->children[i]->draw(depth, glm::scale(glm::vec3(0.5f)));
		}
	}
	*/
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(terr->model));
	//terr->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (showDepth) {
		glUseProgram(render);
		glBindVertexArray(quadVAO);
		//glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		//glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
		//glBindTexture(GL_TEXTURE_2D, textureDepthbuffer);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	else {

		glUseProgram(shadow);
		glUniformMatrix4fv(projectionShadow, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewShadow, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(lightSpaceMatShadow, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		//glm::vec3 lightPos(0.0f, -5.0f, 20.0f);
		glUniform3fv(lightPosShadow, 1, glm::value_ptr(lightPos));
		glUniform3fv(viewPosShadow, 1, glm::value_ptr(eye));
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glEnable(GL_DEPTH_TEST);
		// Now draw into the actual window
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ufoArmyRoot->draw(shadow, glm::mat4(1.0f));

		//processInput(window);




		for (auto currBlaster : blasterArmyRoot->children) {

			Transform* currTransform = (Transform*)currBlaster;

			glm::mat4 tempM = currTransform->M;
			glm::vec3 currPoint(tempM[3][0], tempM[3][1], tempM[3][2]);

			currPoint.z = currPoint.z - blasterSpeed;

			tempM = glm::translate(glm::mat4(1.0f), currPoint);
			currTransform->M = tempM;


		}

		blasterArmyRoot->draw(shadow, glm::mat4(1.0f));


		/*
		if (!toCull)
			root->draw(shadow, glm::mat4(1.0f));

		else {
			for (int i = 0; i < root->children.size(); i++) {
				if (root->children[i]->toDraw)
					root->children[i]->draw(shadow, glm::scale(glm::vec3(0.5f)));
			}
		}
		*/
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(terr->model));
		//terr->draw();
	}
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{	

	// Check for a key press.
	if (action == GLFW_PRESS)
	{

		switch (key)
		{
		case GLFW_KEY_U:
			//Update terrain
			terr->update();
			break;
		case GLFW_KEY_O:
			loadShadows = false;
			showDepth = false;
			break;
		case GLFW_KEY_L:
			loadShadows = true;
			showDepth = true;
			break;
		case GLFW_KEY_M:
			loadShadows = true;
			showDepth = false;
			break;

		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		// USED FOR COLLISION DETECTION DEBUGGING
		case GLFW_KEY_1:
			// Toggle showing bounding sphere
			if (showSphere == false) {
				showSphere = true;
			}

			else {
				showSphere = false;
			}

			sphere->swapShowSphere(showSphere);
			sphere2->swapShowSphere(showSphere);
			
			break;

		// TOGGLE COLLISION DETECTING
		case GLFW_KEY_2:
			if (collision) {
				collision = false;
			}
			else {
				collision = true;
			}
			break;

		// TOGGLE FPS
		case GLFW_KEY_3:
			if (FPS) {
				FPS = false;
			}

			else {
				FPS = true;
			}
			break;
		// TOGGLE FREE CAM
		case GLFW_KEY_4:
			if (freeCam) {
				freeCam = false;
			}

			else {
				freeCam = true;
			}
			break;
		// TOGGLE MOVE MOUSE/ROTATE
		case GLFW_KEY_5:
			if (moveMouse) {
				moveMouse = false;
			}

			else {
				moveMouse = true;
			}
			break;



		// SHOOT a blaster shot
		case GLFW_KEY_SPACE:
		{
			glm::mat4 viewMatrixInverse = glm::inverse(camera.GetViewMatrix());
			glm::vec3 cameraPositionWorldSpace = glm::vec3(viewMatrixInverse[3][0], viewMatrixInverse[3][1], viewMatrixInverse[3][2]);
			glm::mat4 blasterStart = glm::translate(glm::mat4(1.0f), 2.0f*cameraPositionWorldSpace);

			Transform* newBlasterT = new Transform(blasterStart);

			newBlasterT->addChild(blaster);
			newBlasterT->addChild(sphere2); // bounding sphere
			blasterArmyRoot->addChild(newBlasterT);
			
			break;
		}
		default:
			break;
		}

	}

}


void Window::cursorCallback(GLFWwindow* window, double xpos, double ypos) {

	// If we turned FPS off, don't do anything
	if (!FPS) {
		return;
	}

	if (!moveMouse) {
		return;
	}

	if (firstMouse)
	{
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

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	
	// If we turned FPS off, don't do anything
	if (!FPS) {
		return;
	}
	camera.ProcessMouseScroll(yOffset);
	FOVdeg = camera.Zoom;
	Window::projection = glm::perspective(glm::radians(FOVdeg),
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::processInput(GLFWwindow* window)
{
	// If we turned FPS off, don't do anything
	if (!FPS) {
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(3, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(4, 1.0f);

	// If we turned Free cam off, only go on x axis, not z/y too
	if (!freeCam) {
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(1, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(2, 1.0f);
}

void Window::collisionDetection()
{
	// If we turned collision detection off, don't do anything
	if (!collision) {
		return;
	}

	// Get Width of both spheres
	float sphere1R = sphere->getRadius();
	float sphere2R = sphere2->getRadius();
	float distNoCollision = sphere1R + sphere2R - 6.0f;

	int countUFO = 0;
	int countBlaster = 0;
	for (auto currUFO : ufoArmyRoot->children) {
		for (auto currBlaster : blasterArmyRoot->children) {

			// Get center of UFO
			Transform* currUTransform = (Transform*)currUFO;
			mat4 tempUM = currUTransform->M;
			vec3 currPointUFO(tempUM[3][0], tempUM[3][1], tempUM[3][2]);

			// Get center of blaster
			Transform* currBTransform = (Transform*)currBlaster;
			glm::mat4 tempBM = currBTransform->M;
			tempBM = scale(tempBM, vec3(.15));
			glm::vec3 currPointB(tempBM[3][0], tempBM[3][1], tempBM[3][2]);

			// Get distance from UFO to blaster
			float totalDistance = glm::distance(currPointUFO, currPointB);
	
			// if there is a collision, remove both the blaster and the ufo
			if (totalDistance < distNoCollision) {
				ufoArmyRoot->removeChild(countUFO);
				blasterArmyRoot->removeChild(countBlaster);
				score++;
			}
			countBlaster++;
		}
		countUFO++;
		countBlaster = 0;
	}

}




