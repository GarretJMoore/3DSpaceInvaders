#pragma once


#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <ostream>
#include <iostream>

#include "Window.h"
#include "Object.h"


class Cube : public Object
{
private:
	GLuint VAO, VBO, EBO;
	std::vector<glm::vec3> vertices;
	std::vector<glm::ivec3> indices;
	std::vector<unsigned int> indicesF;
	std::vector<std::string> faces;
	unsigned int cubemapTexture;

public:
	
	Cube(float size);
	~Cube();

	void draw();
	void draw2(GLuint);
	void update();

	void spin(float deg);
	unsigned int loadCubemap(std::vector<std::string> faces);
	unsigned int getTexture();
};

#endif

