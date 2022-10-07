#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model.h"
#include "Camera.h"

struct DynamicObjectRotation {
	glm::vec3 rotationAxis;
	float rotationDegrees;
};

class DynamicObject
{
public:
	DynamicObject(Model* model);
	DynamicObject();

	void Translate(glm::vec3 translation);
	void Rotate(float degrees, glm::vec3 rotationAxis);
	void Scale(glm::vec3 scale);

	inline glm::mat4 GetTransform() const { return trans; };

	void AttachModel(const char* modelSource);
	void AttachModel(Model* model);
	glm::mat4 transform;
private:
	glm::mat4 trans, view, projection;
	Model* model;
};