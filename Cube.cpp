#include "Cube.h"
#define STB_IMAGE_IMPLEMENTATION
#include "packages/stb-master/stb_image.h"

Cube::Cube(float size)
{
	
	// Model matrix. Since the original size of the cube is 2, in order to
	// have a cube of some size, we need to scale the cube by size / 2.
	model = glm::scale(glm::vec3(size / 1.f));

	// The color of the cube. Try setting it to something else!
	color = glm::vec3(1.0f, 0.95f, 0.1f);

	vertices.push_back(glm::vec3(-500, 500, 500));
	vertices.push_back(glm::vec3(-500, -500, 500));
	vertices.push_back(glm::vec3(500, -500, 500));
	vertices.push_back(glm::vec3(500, 500, 500));
	vertices.push_back(glm::vec3(-500, 500, -500));
	vertices.push_back(glm::vec3(-500, -500, -500));
	vertices.push_back(glm::vec3(500, -500, -500));
	vertices.push_back(glm::vec3(500, 500, -500));

	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2 
	// and v3 in counter-clockwise order.

	// Front face.
	indices.push_back(glm::ivec3(0, 1, 2));
	indices.push_back(glm::ivec3(2, 3, 0));
	// Back face.
	indices.push_back(glm::ivec3(7, 6, 5));
	indices.push_back(glm::ivec3(5, 4, 7));
	// Right face.
	indices.push_back(glm::ivec3(3, 2, 6));
	indices.push_back(glm::ivec3(6, 7, 3));
	// Left face.
	indices.push_back(glm::ivec3(4, 5, 1));
	indices.push_back(glm::ivec3(1, 0, 4));
	// Top face.
	indices.push_back(glm::ivec3(4, 0, 3));
	indices.push_back(glm::ivec3(3, 7, 4));
	// Bottom face.
	indices.push_back(glm::ivec3(1, 5, 6));
	indices.push_back(glm::ivec3(6, 2, 1));



	faces.push_back("skybox/right.jpg");
	faces.push_back("skybox/left.jpg");
	faces.push_back("skybox/top.jpg");
	faces.push_back("skybox/bottom.jpg");
	faces.push_back("skybox/front.jpg");
	faces.push_back("skybox/back.jpg");

	cubemapTexture = loadCubemap(faces);

	// bind vbo/vao/ebo
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);


}

Cube::~Cube()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Cube::draw()
{
}

void Cube::draw2(GLuint shader)
{
	//Draw skybox.
	glDepthFunc(GL_LEQUAL);
	glm::mat4 skyView = glm::mat4(glm::mat3(Window::view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(skyView));

	glDepthMask(GL_FALSE);

	//Skybox cube.
	glBindVertexArray(VAO);//Bind the vertex.
	glActiveTexture(GL_TEXTURE0);//Enable the texture.
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);//Bind the cubemapTexture.

	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Use clamp to edge to hide skybox edges:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);//Unbind vertex.
	glDepthMask(GL_TRUE);//Turn depth writing back on.

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
}

void Cube::update()
{
	// Spin the cube by 1 degree.
	spin(0.1f);
}

void Cube::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}


unsigned int Cube::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int Cube::getTexture()
{
	return cubemapTexture;
}
