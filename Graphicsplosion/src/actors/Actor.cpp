#include "stdafx.h"
#include "Actor.h"

#include "render/Renderer.h"
#include "render/Model.h"

#include "main/Time.h"

void Actor::Render(class Renderer* renderer, const ShaderProgram* shaderOverride) const {
	// Optionally override the rendering shader
	const ShaderProgram* shader = this->shader;

	if (shaderOverride) {
		shader = shaderOverride;
	}

	if (model && shader) {
		// Setup the shader for rendering
		renderer->UseTexture(texture, shader);
		shader->SetUniform("matWorld", GetTransform());

		// Set the animation frame
		float time = Time::GetTime() - ((int)Time::GetTime() / 3 * 3);

		model->PoseBones(time);

		// Render the model!
		model->Render(*renderer, *shader);
	}
}