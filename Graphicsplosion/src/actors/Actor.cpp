#include "stdafx.h"
#include "Actor.h"

#include "render/Renderer.h"
#include "render/Model.h"

void Actor::Render(class Renderer* renderer) const {
	if (model && shader) {
		shader->SetUniform("matWorld", GetTransform());
		model->Render(*renderer);
	}
}