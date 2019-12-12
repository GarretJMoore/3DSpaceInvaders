#include "PointCloud.h"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize)
	: pointSize(pointSize)
{

	float sWidth = 10.0945;
	/*
	* SECTION 2:
	* The parser for .obj files
	* Currently supports only files with prefix 'v'
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

			// If the line is a face
			// CURRENTLY SUPPORTS FACE LINES IN FORMAT #//# #//# #//#, might need to change in the future for general obj files. Doesn't read //# part

			if (label == "f") {
				glm::ivec3 face;

				string x, y, z;

				ss >> x;
				face.x = std::stoul(x) - 1;

				ss >> y;
				face.y = std::stoul(y) - 1;

				ss >> z;
				face.z = std::stoul(z) - 1;


				//cerr << face.x << " " << face.y << " " << face.z << endl;

				indices.push_back(face);


			}

		}
	}

	else {
		std::cerr << "Can't open the file " << objFilename << endl;
	}


	objFile.close();


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
	for (auto & currPoint : points) {
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



	// adjust all points by subtracting the center points
	for (auto& currPoint2 : points) {
		currPoint2.x = currPoint2.x - centerX;
		currPoint2.y = currPoint2.y - centerY;
		currPoint2.z = currPoint2.z - centerZ;

	}


	// Scale the model

	// default distance to be updated by the first point checked
	float dFromCenter = -INFINITY;

	glm::vec3 cPoint(centerX, centerY, centerZ);

	// Calculate the maximum distance any point is from the center of the model
	for (auto &currPoint : points) {
		float dCurr = glm::length(currPoint);
		if (dCurr > dFromCenter) {
			dFromCenter = dCurr;
		}
	}

	// Get scaling factor
	float scalingFactor = sWidth / dFromCenter;

	// Scale all verticies by the scaling factor
	for (auto &currPoint : points) {
		currPoint = currPoint * scalingFactor;
	}

	// Set materials
	
	// Bunny: High specular, low diffuse (material = green pastic ish)
	if (objFilename == "bunny.obj") {
		ambient.r = .215;
		ambient.g = .0745;
		ambient.b = .0215;

		diffuse.r = .07568;
		diffuse.g = .04240;
		diffuse.b = .07568;

		specular.r = .933;
		specular.g = .727811;
		specular.b = .233;

		shininess = .2;

	}

	// Dragon: High Diffuse, no specular (material = silverish)
	else if (objFilename == "dragon.obj") {
		
		ambient.r = 0.03;
		ambient.g = .49225;
		ambient.b = 0.03;

		diffuse.r = .20754;
		diffuse.g = .90754;
		diffuse.b = .0754;

		specular.r = .02;
		specular.g = .02;
		specular.b = .02;

		shininess = .25;
	}

	// Bear: High Diffuse and specular reflection (material = brass)
	else if (objFilename == "bear.obj") {
		ambient.r = .329412;
		ambient.g = .223529;
		ambient.b = .027451;

		diffuse.r = .780392;
		diffuse.g = .568627;
		diffuse.b = .113725;

		specular.r = .992157;
		specular.g = .941176;
		specular.b = .807843;

		shininess = .21794872;
	}


	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);

	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);


	// Make a second VBO for the normals
	glGenBuffers(1, &normalVBO);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexNormal.size(), vertexNormal.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

PointCloud::~PointCloud()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &normalVBO);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
	
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	spin(0.1f);
}

void PointCloud::updatePointSize(GLfloat size)
{
	/*
	* Section 3: Implement this function to adjust the point size.
	*/

	pointSize = pointSize + size;
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	//model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}


// Scale the model with mouse control
void PointCloud::scrollScale(float yOffset) {

	float scaleValue;
	if (yOffset == 1) {
		scaleValue = 1.1;
	}
	else {
		scaleValue = .9;
	}

	glm::vec3 scalingVector(scaleValue, scaleValue, scaleValue);
	glm::mat4 scalingMatrix = glm::scale(scalingVector);
	model = scalingMatrix * model;

}

glm::vec3 PointCloud::scrollCenter(float yOffset, glm::vec3 modelCenter) {

	float scaleValue;
	if (yOffset == 1) {
		scaleValue = 1.1;
	}
	else {
		scaleValue = .9;
	}

	return glm::vec3(1.0f);
}

void PointCloud::clickRotation(float rotAngle, glm::vec3 rotAxis) {
	
	glm::mat4 temp = glm::rotate(glm::mat4(1.0f), glm::radians(rotAngle), rotAxis);
	model = temp * model;
	
}

void PointCloud::clickOrbit(float rotAngle, glm::vec3 rotAxis)
{
}

glm::vec3 PointCloud::getAmbient()
{
	return ambient;
}

glm::vec3 PointCloud::getDiffuse()
{
	return diffuse;
}

glm::vec3 PointCloud::getSpecular()
{
	return specular;
}

glm::vec3 PointCloud::getCenter()
{
	return center;
}

float PointCloud::getShininess()
{
	return shininess;
}

void PointCloud::setUpLight(glm::vec3 lightPos, glm::vec3 lightColor) {

	color = lightColor;

	model = glm::mat4(1.0f);

	// translate the model
	model = glm::translate(model, lightPos);

	// Scale the model
	model = glm::scale(model, glm::vec3(0.04f));




}
