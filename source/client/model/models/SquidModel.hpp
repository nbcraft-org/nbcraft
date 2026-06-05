// copied from wilyicaro's PEtoLE
#pragma once

#include "Model.hpp"

class SquidModel : public Model
{
public:
	SquidModel();
	~SquidModel();
	void render(float, float, float, float, float, float) override;
	void setupAnim(float, float, float, float, float, float) override;

private:
	ModelPart m_body;
	ModelPart* m_tentacles[8];
};
