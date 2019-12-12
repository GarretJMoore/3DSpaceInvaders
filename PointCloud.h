#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

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
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>



class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> vertexNormal;
	std::vector<glm::ivec3> indices;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::vec3 center;

	float shininess;

	GLuint vao, vbo, ebo, normalVBO, lightVAO;
	GLfloat pointSize;
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg);

	void scrollScale(float yOffset);
	glm::vec3 scrollCenter(float yOffset, glm::vec3 modelCenter);

	void clickRotation(float rotAngle, glm::vec3 rotAxis);
	void clickOrbit(float rotAngle, glm::vec3 rotAxis);

	glm::vec3 getAmbient();
	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();
	glm::vec3 getCenter();
	float getShininess();

	void setUpLight(glm::vec3 lightPos, glm::vec3 lightColor);

	
};

#endif

