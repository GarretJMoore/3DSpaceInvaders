#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"
#include "Node.h"
#include "Transform.h"
#include "Geometry.h"
#include "Plane.h"
#include "Camera.h"
#include "Terrain.h"

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;

	static GLuint SCR_WIDTH;
	static GLuint SCR_HEIGHT;
	
	
	static PointCloud * cubePoints;
	static PointCloud * currentObj;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint program, program2, depth, render, shadow, projectionLoc, viewLoc,
		modelLoc, colorLoc, matAmbLoc, matDifLoc, matSpecLoc, matShinLoc,
		lightSpaceLoc, lightPosShadow, viewPosShadow, modelShadow, viewShadow,
		projectionShadow, lightSpaceMatShadow, depthMapFBO, depthMap, quadVAO, quadVBO;

	static glm::mat4 lightView;
	static glm::mat4 lightProjection;
	static glm::mat4 lightSpaceMatrix;
	static glm::vec3 lightPos;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void shadowMapping();
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorCallback(GLFWwindow* window, double xPos, double yPos);
	static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static bool calcFrustum(glm::vec3 point);

	static glm::vec3 trackBallMapping(glm::vec2 point);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void processInput(GLFWwindow* window);
	static void collisionDetection();

};

#endif
