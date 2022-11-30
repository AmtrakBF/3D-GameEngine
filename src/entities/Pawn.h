#pragma once
#include "entities/WorldEntity.h"

class Pawn : public WorldEntity
{
public:
	Pawn(Model model, glm::vec3 position);
};