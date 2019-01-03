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

	// Render the model
	if (model && shader) {
		// Setup the shader for rendering
		// Set textures
		renderer->UseTexture(texture, shader, "textureSampler", 3);
		renderer->UseTexture(normalMapTexture, shader, "normalSampler", 2);

		shader->SetUniform("isNormalMapEnabled", normalMapTexture != nullptr ? 1 : 0);

		// Set transform
		shader->SetUniform("matWorld", GetTransform());

		// Set the animation frame
		float time = Time::GetTime() - ((long long int)Time::GetTime() / 3 * 3);

		model->PoseBones(time);

		// Render the model!
		model->Render(*renderer, *shader);
	}
}