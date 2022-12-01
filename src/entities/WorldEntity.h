#pragma once
#include "glm/glm.hpp"

#include <string>
#include <vector>

#include "rendering/Model.h"
#include "collision/CollisionBox.h"

class WorldEntity
{
private:

protected:
	uint32_t m_Id;

public:
	WorldEntity();

	std::string m_Name;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::vec3 m_Direction;

	glm::vec3 m_CollisionMin;
	glm::vec3 m_CollisionMax;

	bool b_UseCollision;
	bool b_IsAttachedToEntity;
	WorldEntity* m_EntityAttchedTo;
	Model m_Model;

	glm::mat4 m_TranslationMatrix, m_ViewMatrix, m_ProjectionMatrix;

	std::vector<CollisionBox> v_CollisionBoxes;

	void SetPosition(glm::vec3 position);

	virtual void Translate(glm::vec3 translation);
	virtual void Rotate(float degrees, glm::vec3 rotationAxis, glm::vec3 offset = {0.0f, 0.0f, 0.0f});
	virtual void Scale(glm::vec3 scale);

	virtual void TranslateCollisionData(glm::vec3 translation);
	virtual void RotateCollisionData(glm::vec3 translation, float degrees, glm::vec3 rotationAxis);
	virtual void ScaleCollisionData(glm::vec3 scale);

	void UpdateEntityMinMax();
	void Delete();
	void AttachModel(Model& model, GLenum drawType);

	uint32_t GetId();

};