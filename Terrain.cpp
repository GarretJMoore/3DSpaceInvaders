#include "Terrain.h"
#include "Geometry.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <typeinfo>

//#include "Terrain.h"

Terrain::Terrain()
{
	// Model matrix. Since the original size of the Terrrain is 2, in order to
	// have a Terrrain of some size, we need to scale the Terrrain by size / 2.
	model = glm::mat4(1);
	height_offset = -10;
	std::set<GLfloat> s = {};
	map = std::vector<std::vector<GLfloat>>(1025, std::vector<GLfloat>(1025, height_offset));
	srand(seed);
	// The color of the Terrain. Try setting it to something else!
	color = glm::vec3(1.0f, 0.0f, 0.1f);

	CHUNK_X = 1025;
	CHUNK_Z = 1025;
	/**** diamond-square ****/
	diamondSquare(map, 1024);
	/**** diamond-square ****/
	int iter = 0;
	for (int i = 0; i < map.size() - 1; i += 1) {
		for (int j = 0; j < map[0].size() - 1; j += 1) {
			glm::vec3 p1 = glm::vec3(i, map[i][j], j);
			glm::vec3 p2 = glm::vec3(i + 1, map[i + 1][j], j);
			glm::vec3 p3 = glm::vec3(i + 1, map[i + 1][j + 1], j + 1);
			glm::vec3 p4 = glm::vec3(i, map[i][j + 1], j + 1);

			points.push_back(p1);
			points.push_back(p2);
			points.push_back(p3);
			points.push_back(p4);

			normals.push_back(glm::cross((p2 - p1), (p3 - p1)));
			normals.push_back(glm::cross((p2 - p1), (p3 - p1)));
			normals.push_back(glm::cross((p3 - p1), (p4 - p1)));
			normals.push_back(glm::cross((p3 - p1), (p4 - p1)));

			faces.push_back(glm::ivec3(iter, iter + 1, iter + 2));
			faces.push_back(glm::ivec3(iter, iter + 2, iter + 3));

			iter += 4;
		}
	}

	float half = 1024 / 2.0f;
	model[3].x -= half;
	model[3].z -= half;
	//model[0].x *= 3;
	//model[2].z *= 3;

	//model *= 0.3f;

	model[1].y = 0.3f;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &vbo2);
	glGenBuffers(1, &ebo);
	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);


	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
		normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(),
		faces.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0.
	// We will be able to access points through it.

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);

}

Terrain::~Terrain()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &vbo1);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Terrain::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Terrain::update()
{
	//seed = time(NULL);
	seed++;
	srand(seed);

	map.clear();
	points.clear();
	normals.clear();
	faces.clear();

	map = std::vector<std::vector<GLfloat>>(1025, std::vector<GLfloat>(1025, height_offset));
	diamondSquare(map, 1024);

	int iter = 0;
	for (int i = 0; i < map.size() - 1; i += 1) {
		for (int j = 0; j < map[0].size() - 1; j += 1) {
			glm::vec3 p1 = glm::vec3(i, map[i][j], j);
			glm::vec3 p2 = glm::vec3(i + 1, map[i + 1][j], j);
			glm::vec3 p3 = glm::vec3(i + 1, map[i + 1][j + 1], j + 1);
			glm::vec3 p4 = glm::vec3(i, map[i][j + 1], j + 1);

			points.push_back(p1);
			points.push_back(p2);
			points.push_back(p3);
			points.push_back(p4);

			normals.push_back(glm::cross((p2 - p1), (p3 - p1)));
			normals.push_back(glm::cross((p2 - p1), (p3 - p1)));
			normals.push_back(glm::cross((p3 - p1), (p4 - p1)));
			normals.push_back(glm::cross((p3 - p1), (p4 - p1)));

			faces.push_back(glm::ivec3(iter, iter + 1, iter + 2));
			faces.push_back(glm::ivec3(iter, iter + 2, iter + 3));

			iter += 4;
		}
	}

	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
		normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(),
		faces.data(), GL_STATIC_DRAW);

}


void Terrain::diamondSquare(std::vector<std::vector<GLfloat>>& Array, int size)
{
	int half = size / 2;
	if (half < 1)
		return;
	//square steps
	for (int z = half; z < CHUNK_Z; z += size)
		for (int x = half; x < CHUNK_X; x += size)
			squareStep(Array, x % CHUNK_X, z % CHUNK_Z, half);
	// diamond steps
	int col = 0;
	for (int x = 0; x < CHUNK_X; x += half)
	{
		col++;
		//If this is an odd column.
		if (col % 2 == 1)
			for (int z = half; z < CHUNK_Z; z += size)
				diamondStep(Array, x % CHUNK_X, z % CHUNK_Z, half);
		else
			for (int z = 0; z < CHUNK_Z; z += size)
				diamondStep(Array, x % CHUNK_X, z % CHUNK_Z, half);
	}
	diamondSquare(Array, size / 2);
}

void Terrain::squareStep(std::vector<std::vector<GLfloat>>& Array, int x, int z, int reach)
{
	int count = 0;
	float avg = 0.0f;
	if (x - reach >= 0 && z - reach >= 0)
	{
		avg += Array[x - reach][z - reach];
		count++;
	}
	if (x - reach >= 0 && z + reach < CHUNK_Z)
	{
		avg += Array[x - reach][z + reach];
		count++;
	}
	if (x + reach < CHUNK_X && z - reach >= 0)
	{
		avg += Array[x + reach][z - reach];
		count++;
	}
	if (x + reach < CHUNK_X && z + reach < CHUNK_Z)
	{
		avg += Array[x + reach][z + reach];
		count++;
	}
	avg += random(reach);
	avg /= count;
	Array[x][z] = avg;
}

void Terrain::diamondStep(std::vector<std::vector<GLfloat>>& Array, int x, int z, int reach)
{
	int count = 0;
	float avg = 0.0f;
	if (x - reach >= 0)
	{
		avg += Array[x - reach][z];
		count++;
	}
	if (x + reach < CHUNK_X)
	{
		avg += Array[x + reach][z];
		count++;
	}
	if (z - reach >= 0)
	{
		avg += Array[x][z - reach];
		count++;
	}
	if (z + reach < CHUNK_Z)
	{
		avg += Array[x][z + reach];
		count++;
	}
	avg += random(reach);
	avg /= count;
	Array[x][z] = avg;
}
