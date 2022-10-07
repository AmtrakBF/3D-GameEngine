#include "DynamicObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

DynamicObject::DynamicObject(Model* model)
	: trans(glm::mat4(1.0f)), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)), transform(glm::mat4(1.0f)), model(model)
{
}

DynamicObject::DynamicObject()
	: trans(glm::mat4(1.0f)), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)), transform(glm::mat4(1.0f))
{
}

void DynamicObject::Translate(glm::vec3 translation)
{
	if (!model)
		return;
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, translation);
	for (int x = 0; x < model->vertices.size(); x++)
	{
		model->vertices[x].vertices = trans * glm::vec4(model->vertices[x].vertices, 1.0f);
	}
	model->VBO.UpdateBuffer(&model->vertices[0], model->GetSizeInBytes());
}

void DynamicObject::Rotate(float degrees, glm::vec3 rotationAxis)
{
	if (!model)
		return;

	trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(degrees), rotationAxis);
	for (int x = 0; x < model->vertices.size(); x++)
	{
		model->vertices[x].vertices = trans * glm::vec4(model->vertices[x].vertices, 1.0f);

		model->vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(trans))) * model->vertices[x].normals;
	}
	model->VBO.UpdateBuffer(&model->vertices[0], model->GetSizeInBytes());
}

void DynamicObject::Scale(glm::vec3 scale)
{ 
	if (!model)
		return;

	bool nonUniform = false;
	if (scale.x != scale.y && scale.y != scale.z)
		nonUniform = true;

	trans = glm::mat4(1.0f);
	trans = glm::scale(trans, scale);
	for (int x = 0; x < model->vertices.size(); x++)
	{
		model->vertices[x].vertices = trans * glm::vec4(model->vertices[x].vertices, 1.0f);

		if (nonUniform)
			model->vertices[x].normals = glm::mat3(glm::transpose(glm::inverse(trans))) * model->vertices[x].normals;
	}
	model->VBO.UpdateBuffer(&model->vertices[0], model->GetSizeInBytes());
}

void DynamicObject::AttachModel(const char* modelSource)
{
	// read from file, create Model
	model->LoadModel(modelSource);
}

void DynamicObject::AttachModel(Model* model)
{
	this->model = model;
}
