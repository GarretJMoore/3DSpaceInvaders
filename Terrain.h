#ifndef Terrain_h
#define Terrain_h
#define STB_IMAGE_IMPLEMENTATION
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <set>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class Terrain {
private:
	GLuint vao, vbo1, vbo2, ebo;

public:
	glm::mat4 model;
	glm::vec3 color;
	unsigned int seed = 3;
	int CHUNK_X, CHUNK_Z;
	GLfloat height_offset;
	std::vector<std::vector<GLfloat>> map;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> faces;
	Terrain();
	~Terrain();

	void draw();
	void update();
	void diamondSquare(std::vector<std::vector<GLfloat>>& Array, int size);
	void diamondStep(std::vector<std::vector<GLfloat>>& Array, int x, int z, int reach);
	void squareStep(std::vector<std::vector<GLfloat>>& Array, int x, int z, int reach);
	inline static float random(float range) {
		range *= 0.5f;
		float r = ((float)rand() / (RAND_MAX));
		return (r * (range * 2)) - range;
	};
};

#endif /* Terrain_h */

