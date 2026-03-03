#pragma once

#include "Model.hpp"

class GhastModel : public Model
{
public:
	GhastModel();
	~GhastModel();
	void render(float, float, float, float, float, float) override;
	void setupAnim(float, float, float, float, float, float) override;

private:
	ModelPart m_body;
	ModelPart* m_tentacles[9];
};
