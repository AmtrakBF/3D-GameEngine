#include "Pawn.h"
#include "glm/gtc/matrix_transform.hpp"

Pawn::Pawn(Model model, glm::vec3 position)
{
	AttachModel(model, GL_STATIC_DRAW);

	b_UseCollision = true;

	Translate(position);
}
