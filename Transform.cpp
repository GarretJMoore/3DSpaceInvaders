#include "Transform.h"

Transform::Transform(glm::mat4 matrix)
{
	shouldDraw = true;
	M = matrix;

	currDirection = 0;
	prevDirection = 2;
	numChanges = 0;
	changeDirection = glfwGetTime();
	
}


Transform::~Transform()
{
}

void Transform::addChild(Node * child) {

	child->parent = this;
	children.push_back(child);


}

void Transform::removeChild(int position)
{
	auto it = children.begin();
	advance(it, position);
	children.erase(it);
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C) {
	
	if (shouldDraw == false) {
		return;
	}

	glm::mat4 newMatrix = C * M;

	for (auto &currNode : children) {
		currNode->draw(shaderProgram, newMatrix);
	}

}

void Transform::update(glm::mat4 C) {

	M = C;

	// Update all children transform nodes
	for (auto &currNode : children) {
		currNode->update(C);
	}



}

std::list<Node*> Transform::getChildren()
{
	return children;
}
