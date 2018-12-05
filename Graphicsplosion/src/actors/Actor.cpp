#include "stdafx.h"
#include "Actor.h"

#include "render/Renderer.h"
#include "render/Model.h"

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

		// Render the model!
		model->Render(*renderer, *shader);
	}
}