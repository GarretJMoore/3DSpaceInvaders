#include "Geometry.h"

Geometry::Geometry(string objFilename, GLfloat pointsize, string nameG, bool isSph, bool showSph)
	: pointSize(pointsize)
{
	sWidth = 10.0945;
	name = nameG;
	isSphere = isSph;
	showSphere = showSph;
	radius = 0;



	/*
	* The parser for .obj files
	*/

	ifstream objFile(objFilename); // The obj file we are reading.


	// Make sure the file can actually be opened
	if (objFile.is_open()) {

		string line; // A line in the file

		// Read all lines of the file
		while (std::getline(objFile, line)) {

			stringstream ss;
			ss << line;

			string label;
			ss >> label;

			// If the line is a vertex
			if (label == "v") {

				// Get the X, Y, Z from ss and put it in point
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;


				// Process the points
				points.push_back(point);
			}

			// If the line is a vertex normal
			if (label == "vn") {

				// Get the X, Y, Z from ss and put it in a point
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;

				// Process the points and push it to VertexNormal
				vertexNormal.push_back(normal);

			}

			// If the line is a face with format f v1/t1/vn1 v2/t2/vn2 v3/t3/vn3

			if (label == "f") {

				string v, vt, vn, tempX, tempY, tempZ;
				

				glm::ivec3 vVertex;
				glm::ivec3 tVertex;
				glm::ivec3 vnVertex;

				//X values
				ss >> tempX;
				replace(tempX.begin(), tempX.end(), '/', ' ');
				
				istringstream ssX(tempX);
				ssX >> v;
				ssX >> vt;
				ssX >> vn;

				vVertex.x = std::stoul(v) - 1;
				
				tVertex.x = std::stoul(vt) - 1;
	
				vnVertex.x = std::stoul(vn) - 1;

				// Y values
				ss >> tempY;
				replace(tempY.begin(), tempY.end(), '/', ' ');

				istringstream ssY(tempY);
				ssY >> v;
				ssY >> vt;
				ssY >> vn;


				vVertex.y = std::stoul(v) - 1;

				tVertex.y = std::stoul(vt) - 1;

				vnVertex.y = std::stoul(vn) - 1;

				// Z values
				ss >> tempZ;
				replace(tempZ.begin(), tempZ.end(), '/', ' ');
				
				istringstream ssZ(tempZ);
				ssZ >> v;
				ssZ >> vt;
				ssZ >> vn;

				vVertex.z = std::stoul(v) - 1;

				tVertex.z = std::stoul(vt) - 1;

				vnVertex.z = std::stoul(vn) - 1;

				// add to indicies in order v, vn, vt
				vertexI.push_back(vVertex.x);
				vertexI.push_back(vVertex.y);
				vertexI.push_back(vVertex.z);

				normalI.push_back(vnVertex.x);
				normalI.push_back(vnVertex.y);
				normalI.push_back(vnVertex.z); 
				
				/*cerr << objFilename << endl;
				cerr << vVertex.x + 1 << " " << vVertex.y + 1 << " " << vVertex.z + 1 << endl;
				cerr << vnVertex.x +1<< " " << vnVertex.y +1 << " " << vnVertex.z + 1 << endl;
				cerr << endl; */
				
			}

		}
	}

	else {
		std::cerr << "Can't open the file " << objFilename << endl;
	}


	objFile.close();

	for (unsigned i = 0; i < vertexI.size(); i++) {

		unsigned int vertexPos = vertexI[i];
		unsigned int normalPos = normalI[i];

		aPoints.push_back(points[ vertexPos ]);
		aNormal.push_back(vertexNormal[ normalPos ]);
		
		indices.push_back(i);
	}
	

	// Center Model
	centerModel();

	// Scale the model
	scaleModel();

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1.0f);
	// Set the color. 
	color = glm::vec3(.2, .9, .1);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * aPoints.size(), aPoints.data(), GL_STATIC_DRAW);

	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);


	// Make a second VBO for the normals
	glGenBuffers(1, &normalVBO);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * aNormal.size(), aNormal.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);



}

Geometry::~Geometry()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &normalVBO);
	glDeleteVertexArrays(1, &vao);

}

void Geometry::draw(GLuint shaderProgram, glm::mat4 C) {

	
	// Update modelView matrix

	model = C;

	if (name == "sphere2") {
		
		model = scale(model, vec3(.15f));
		radius = radius * .15;

	}
	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// Bind to the VAO.
	glBindVertexArray(vao);

	if (isSphere == false) {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else if (isSphere == true && showSphere == true) {
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	// Unbind from the VAO.
	glBindVertexArray(0);

	
	
	
}


void Geometry::update(glm::mat4 C) {


}

void Geometry::swapShowSphere(bool changeTo)
{
	showSphere = changeTo;
}

void Geometry::scaleModel()
{

	// default distance to be updated by the first point checked
	float dFromCenter = -INFINITY;

	glm::vec3 cPoint(center.x, center.y, center.z);

	// Calculate the maximum distance any point is from the center of the model
	for (auto &currPoint : aPoints) {
		float dCurr = glm::length(currPoint);
		if (dCurr > dFromCenter) {
			dFromCenter = dCurr;
		}
	}

	// Get scaling factor
	float scalingFactor = sWidth / dFromCenter;
	radius = dFromCenter;

	// Scale all verticies by the scaling factor
	for (auto &currPoint : aPoints) {
		currPoint = currPoint * scalingFactor;
	}

}

void Geometry::centerModel()
{

	/*
	* Center the model
	*/
	float maxX = -INFINITY;
	float maxY = -INFINITY;
	float maxZ = -INFINITY;

	float minX = INFINITY;
	float minY = INFINITY;
	float minZ = INFINITY;

	// Go through all points and find the min/max point
	for (auto & currPoint : aPoints) {
		// Check max points first
		if (currPoint.x > maxX) {
			maxX = currPoint.x;
		}

		if (currPoint.y > maxY) {
			maxY = currPoint.y;
		}

		if (currPoint.z > maxZ) {
			maxZ = currPoint.z;
		}

		// Check all min points
		if (currPoint.x < minX) {
			minX = currPoint.x;
		}
		if (currPoint.y < minY) {
			minY = currPoint.y;
		}
		if (currPoint.z < minZ) {
			minZ = currPoint.z;
		}
	}
	// Get center point
	float centerX = (minX + maxX) / 2;
	float centerY = (minY + maxY) / 2;
	float centerZ = (minZ + maxZ) / 2;

	center.x = centerX;
	center.y = centerY;
	center.z = centerZ;

	// adjust all points by subtracting the center points
	for (auto& currPoint2 : aPoints) {
		currPoint2.x = currPoint2.x - centerX;
		currPoint2.y = currPoint2.y - centerY;
		currPoint2.z = currPoint2.z - centerZ;
		

	}

}

float Geometry::getRadius()
{
	return radius;
}
