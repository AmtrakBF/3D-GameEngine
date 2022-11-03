#include "debug/Debug.h"
#include "rendering/Camera.h"
#include <vector>

#include "rendering/Model.h"
#include "rendering/Renderer.h"

Debug* Debug::Instance()
{
	static Debug instance;
	return &instance;
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! ----------------------------------------------------------- MEMORY LEAK NEEDS TO BE FIXED IM JUST LAZY RN ------------------------------------------------
//! Also will probably do this a better way, so instead of creating a box each time we can just use some matrix to transform the location? IDK
void Debug::DrawCollisions(WorldEntity* entity)
{
	if (!entity)
		return;

	for (auto& i : CollisionBoxes)
	{
		Renderer::Entities.remove(i);
	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::uvec3> indices;

	int offset = 0;

	for (const auto& x : entity->v_CollisionBoxes)
	{
		//! --- Front Vertices ---
		
		glm::vec3 max = x.CollisionMax();
		glm::vec3 min = x.CollisionMin();

		//! BOTTOM LEFT
		vertices.push_back(min);
		//! BOTTOM RIGHT
		vertices.push_back({ max.x, min.y, min.z });
		//! TOP LEFT
		vertices.push_back({ min.x, max.y, min.z });
		//! TOP RIGHT
		vertices.push_back({ max.x, max.y, min.z });

		//! --- Rear Vertices ---

		//! BOTTOM LEFT
		vertices.push_back({ min.x, min.y, max.z });
		//! BOTTOM RIGHT
		vertices.push_back({ max.x, min.y, max.z });
		//! TOP LEFT
		vertices.push_back({ min.x, max.y, max.z });
		//! TOP RIGHT
		vertices.push_back({ max.x, max.y, max.z });


		//! every pair is a side { Front / Back / Right / Left / Top / Bottom }
		indices.push_back({ 0 + offset, 1 + offset, 2 + offset });
		indices.push_back({ 1 + offset, 2 + offset, 3 + offset });
		indices.push_back({ 4 + offset, 5 + offset, 6 + offset });
		indices.push_back({ 5 + offset, 6 + offset, 7 + offset });
		indices.push_back({ 3 + offset, 7 + offset, 5 + offset });
		indices.push_back({ 5 + offset, 1 + offset, 3 + offset });
		indices.push_back({ 2 + offset, 6 + offset, 4 + offset });
		indices.push_back({ 4 + offset, 0 + offset, 2 + offset });
		indices.push_back({ 6 + offset, 3 + offset, 7 + offset });
		indices.push_back({ 6 + offset, 3 + offset, 2 + offset });
		indices.push_back({ 5 + offset, 0 + offset, 1 + offset });
		indices.push_back({ 5 + offset, 0 + offset, 4 + offset });

		offset += 8;

		WorldEntity* collisionEntity = new WorldEntity();
		Model model{ entity->m_Model.m_Shader, &vertices, &indices };
		collisionEntity->AttachModel(model, GL_DYNAMIC_DRAW);
		CollisionBoxes.push_back(collisionEntity);
	}

	PRINT(CollisionBoxes.size());
}

Debug::~Debug()
{

}

void Debug::DebugCircle(glm::vec3 position, float radius)
{

}

// Draw collisions