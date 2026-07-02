#pragma once

#include "Model.hpp"

class SignModel : public Model
{
public:
	SignModel();
	~SignModel();

	void render(float time = 0, float r = 0, float bob = 0, float yRot = 0, float xRot = 0, float scale = 0) override;

public:
	ModelPart m_cube;
	ModelPart m_cube2;
};
