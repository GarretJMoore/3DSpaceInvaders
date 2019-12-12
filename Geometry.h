#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "Node.h"
#include "Transform.h"
#include "Window.h"


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
class Geometry :
	public Node
{

private:
	std::vector<glm::vec3> points;

	std::vector<glm::vec3> vertexNormal;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> vertexI;
	std::vector<unsigned int> normalI;

	std::vector<glm::vec3> aPoints;
	std::vector<glm::vec3> aNormal;

	std::vector<glm::vec2> textures;

	glm::mat4 model;
	
	glm::vec3 color;


	

	float sWidth;
	bool isSphere;
	bool showSphere;
	string name;

	GLuint vao, vbo, ebo, normalVBO, lightVAO, textureVBO;
	GLfloat pointSize;


public:
	glm::vec3 center;
	float radius;


	Geometry(string objFilename, GLfloat pointsize, string name, bool isSph, bool showSph);
	~Geometry();
	
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);
	void swapShowSphere(bool changeTo);

	void scaleModel();
	void centerModel();

	float getRadius();

};
#endif
