#ifndef NODE_H_
#define NODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include "Object.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>


using namespace std;
using namespace glm;
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

class Node
{
private:
	string name;
	

public:
	Node * parent;
	bool shouldDraw;
	Node();
	~Node();
	virtual void update(glm::mat4 C) = 0;
	virtual void draw(GLuint shaderProgram, glm::mat4 C) = 0;
};
#endif

