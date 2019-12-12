#pragma once
#include "Window.h"
class Plane
{
public:
	glm::vec3 normal;
	glm::vec3 point;
	float d;

	Plane(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3);
	~Plane();

	void set3Points(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	float distance(glm::vec3 p);
};

