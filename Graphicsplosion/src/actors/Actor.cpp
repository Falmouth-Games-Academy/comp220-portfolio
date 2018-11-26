#include "stdafx.h"
#include "Actor.h"

void Actor::Render() {
	// Draw the two models
	// Draw the pigeon
	/*matWorld = glm::identity<glm::mat4>();
	glm::mat4 pigeonTransform = matWorld;
	glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, glm::value_ptr(pigeonTransform));
	dbgModel.Render(render);

	// Draw the bunny
	glm::mat4 bunnyTransform = glm::scale(glm::vec3(0.15f, 0.15f, 0.15f)) * matWorld/* * glm::rotate(glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f))*//*;
	bunnyTransform = glm::translate(glm::vec3(15.0f, 0.0f, 0.0f)) * bunnyTransform;

	glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, glm::value_ptr(bunnyTransform));
	dbgSecondModel.Render(render);*/
}