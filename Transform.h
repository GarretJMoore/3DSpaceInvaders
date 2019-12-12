#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Node.h"
#include "Window.h"
#include <list>


class Transform :
	public Node
{
private:
	
	
	

public:
	std::list<Node*> children;
	glm::mat4 M;

	int currDirection;
	int prevDirection;
	int numChanges;
	float changeDirection;
	
	Transform(glm::mat4 matrix);
	~Transform();


	void addChild(Node * child);
	void removeChild(int position);
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);

	Transform * parent;
	std::list<Node*> getChildren();
};
#endif

