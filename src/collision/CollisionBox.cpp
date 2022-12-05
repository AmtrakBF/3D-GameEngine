#include "collision/CollisionBox.h"
#include "entities/WorldEntity.h"

#include "glm/gtc/matrix_transform.hpp"

CollisionBox::CollisionBox(WorldEntity* entity, CollisionData data)
	: entity(entity), m_CollisionDirection(0.0f), c_OriginCollisionDimensions(data.dimensions)
{
	if (!entity)
		throw "ERROR::COLLISIONBOX::INSTANTIATION::ENTITY::NULL";

	//! Set minimum and maximum collision points 
	m_CollisionMin = entity->m_Position + data.min;
	m_CollisionMax = entity->m_Position + data.max;

	//! Center of collision box
	m_CollisionCenter = { (m_CollisionMax.x + m_CollisionMin.x) / 2, (m_CollisionMax.y + m_CollisionMin.y) / 2, (m_CollisionMax.z + m_CollisionMin.z) / 2 };

	//! Set collision position, subject to change
	m_CollisionPos = m_CollisionMin;

	//! Set collision dimensions
	m_CollisionDimensions = data.dimensions;
}

void CollisionBox::Rotate(float degrees, glm::vec3 rotationAxis)
{
	glm::mat4 RotationMatrix = glm::mat4(1.0f);
	RotationMatrix = glm::rotate(RotationMatrix, glm::radians(degrees), rotationAxis);

	m_CollisionMax = RotationMatrix * glm::vec4(m_CollisionMax, 1.0f);
	m_CollisionMin = RotationMatrix * glm::vec4(m_CollisionMin, 1.0f);

	//! Max is always top right
	//! Min is always bottom left

	//! if Min.x is > Max.x
	//! Swap around so the Max is actually the Max
	if (m_CollisionMin.x > m_CollisionMax.x)
	{
		float temp = m_CollisionMax.x;
		m_CollisionMax.x = m_CollisionMin.x;
		m_CollisionMin.x = temp;
	}

	//! if Min.y is > Max.y
	//! Swap around so the Max is actually the Max
	if (m_CollisionMin.y > m_CollisionMax.y)
	{
		float temp = m_CollisionMax.y;
		m_CollisionMax.y = m_CollisionMin.y;
		m_CollisionMin.y = temp;
	}

	//! Calculate new collision lengths with rotation
	m_CollisionDimensions = glm::vec3(abs(m_CollisionMax.x) + abs(m_CollisionMin.x), abs(m_CollisionMax.y) + abs(m_CollisionMin.y), abs(m_CollisionMax.z) + abs(m_CollisionMin.z));

// 	//! Set position back to position
// 	m_CollisionMax += entity->m_Position;
// 	m_CollisionMin += entity->m_Position;

	//! Calculate center of collision
	m_CollisionCenter = { (m_CollisionMax.x + m_CollisionMin.x) / 2, (m_CollisionMax.y + m_CollisionMin.y) / 2, (m_CollisionMax.z + m_CollisionMin.z) / 2 };
}

void CollisionBox::Translate(glm::vec3 position)
{
	m_CollisionMin += position;
	m_CollisionMax += position;
	m_CollisionCenter += position;
}

void CollisionBox::Scale(glm::vec3 scale)
{
	m_CollisionMax *= scale;
	m_CollisionMin *= scale;

	//! Calculate new collision lengths with rotation
	m_CollisionDimensions = glm::vec3(abs(m_CollisionMax.x) + abs(m_CollisionMin.x), abs(m_CollisionMax.y) + abs(m_CollisionMin.y), abs(m_CollisionMax.z) + abs(m_CollisionMin.z));

	//! Calculate center of collision
	m_CollisionCenter = { (m_CollisionMax.x + m_CollisionMin.x) / 2, (m_CollisionMax.y + m_CollisionMin.y) / 2, (m_CollisionMax.z + m_CollisionMin.z) / 2 };
}
