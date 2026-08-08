#pragma once

#include "Model.hpp"

class SignModel : public Model
{
public:
	SignModel();
	~SignModel();

	void render(float time = 0.0f, float r = 0.0f, float bob = 0.0f, float yRot = 0.0f, float xRot = 0.0f, float scale = 0.0f) override;

public:
	ModelPart m_cube;
	ModelPart m_cube2;
};
