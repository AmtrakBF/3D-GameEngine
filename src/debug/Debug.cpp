#include "debug/Debug.h"
#include "rendering/Camera.h"
//#include "entities/Pawn.h"

Debug* Debug::Instance()
{
	static Debug instance;
	return &instance;
}

Debug::~Debug()
{
// 	if (pawn) delete pawn;
// 	if (model) delete model;
// 	if (shader) delete shader;
}

void Debug::DebugCircle(glm::vec3 position, float radius)
{
// 	shader = new Shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
// 	shader->use();
// 	shader->SetVec4("color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
// 
// 	glm::mat4 shaderModel = glm::mat4(1.0f);
// 	shader->SetMat4("model", shaderModel);
// 
// 	glm::mat4 view = glm::mat4(1.0f);
// 	view = Camera::Instance()->GetView(); // position, target, and up -- no need to calculate right and above
// 	shader->SetMat4("view", view);
// 
// 	glm::mat4 projection;
// 	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
// 	shader->SetMat4("projection", projection);
// 
// 	model = new Model("res/objects/testobj.obj", *shader);
// 	pawn = new Pawn(*model, position);
}
