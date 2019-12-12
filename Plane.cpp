#include "Plane.h"

/* Most of this code comes from:
http://www.lighthouse3d.com/tutorials/view-frustum-culling/
*/

Plane::Plane(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3)
{
	set3Points(P1, P2, P3);
}


Plane::~Plane()
{
}

void Plane::set3Points(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{

	glm::vec3 point1 = v1 - v2;
	glm::vec3 point2 = v3 - v2;

	normal = normalize(point1 * point2);
	point = v2;
	
	d = -1.0f * (glm::dot(normal, point));

}

float Plane::distance(glm::vec3 p)
{
	return (d + glm::dot(normal, p));
}
