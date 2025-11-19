#pragma once
#include<glm.hpp>

class Cube {
public:
	Cube();
	~Cube();

	void Create();

	void SetPosition(float x, float y, float z);

	glm::mat4 GetPosition() const;

	void Draw();

private:
	unsigned int VAO;
	unsigned int EBO;
	unsigned int positionVBO;
	unsigned int colorVBO;

	glm::vec3 position;
	glm::mat4 model;

	void CreateBuffers();
	void FillBuffers();
	void LinkBuffers();

	void UpdatePosition();
};