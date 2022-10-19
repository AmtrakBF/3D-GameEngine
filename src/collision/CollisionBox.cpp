#include "collision/CollisionBox.h"
#include "entities/WorldEntity.h"

CollisionBox::CollisionBox(WorldEntity* entity, glm::vec3 dimensions, glm::vec3 position)
	: entity(entity)
{
	if (!entity)
		throw "ERROR::COLLISIONBOX::INSTANTIATION::ENTITY::NULL";

	//! Set collision position, defaulted to (-1, -1, 0)
	m_CollisionPos = position;

	//! Set minimum and maximum collision points 
	m_CollisionMin = m_CollisionPos + entity->m_Position;
	m_CollisionMax = m_CollisionPos + entity->m_Position + dimensions;

	//! Center of collision box
	m_CollisionCenter = { dimensions.x / 2, dimensions.y / 2, dimensions.z / 2 };
	m_CollisionCenter += m_CollisionMin;

	//! Set collision position, subject to change
	m_CollisionPos = { -1.0f, -1.0f, 0 };

	//! Set collision dimensions
	m_CollisionDimensions = dimensions;
}

void CollisionBox::Rotate()
{
	//! Only rotates on z axis...
	float radian = (entity->m_Rotation.z * 3.14159265359f) / 180.0f;

	//! Set position to origin 
	m_CollisionMax -= entity->m_Position;
	m_CollisionMin -= entity->m_Position;

	//! Objects default transform width * cos(rotation in radians) + opposite(CollisionPosition.x)
	m_CollisionMax.x = glm::round(((5 * cos(radian)) + (-m_CollisionPos.x)));
	//! Objects default transform width * sin(rotation in radians) + opposite(CollisionPosition.y)
	m_CollisionMax.y = glm::round(((5 * sin(radian)) + (-m_CollisionPos.y)));

	//! Objects default transform opposite(Height) * cos(rotation in radians) + CollisionPosition.y
	m_CollisionMin.x = glm::round((((-2) * cos(radian)) + m_CollisionPos.x));
	//! Objects default transform opposite(Height) * sin(rotation in radians) + CollisionPosition.y
	m_CollisionMin.y = glm::round((((-2) * sin(radian)) + m_CollisionPos.y));

	//! Rotates the collision position for proper alignment of Min and Max 
	m_CollisionPos.x = glm::round((((-2) * cos(radian)) + m_CollisionPos.x));
	m_CollisionPos.y = glm::round((((-2) * sin(radian)) + m_CollisionPos.y));

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

	//! Set position back to position
	m_CollisionMax += entity->m_Position;
	m_CollisionMin += entity->m_Position;

	//! Calculate center of collision
	m_CollisionCenter = { (m_CollisionMax.x + m_CollisionMin.x) / 2, (m_CollisionMax.y + m_CollisionMin.y) / 2, (m_CollisionMax.z + m_CollisionMin.z) / 2 };
}

void CollisionBox::Update(glm::vec3 position)
{
	m_CollisionMin += position;
	m_CollisionMax += position;
	m_CollisionCenter += position;
}
