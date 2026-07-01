#pragma once

#include "Model.hpp"

class WolfModel : public Model
{
public:
	WolfModel();
	~WolfModel();

	void render(float, float, float, float, float, float) override;
	void setupAnim(float, float, float, float, float, float) override;
	void prepareMobModel(const Mob&, float, float, float) override;

protected:
	ModelPart m_head;
	ModelPart m_body;
	ModelPart m_leg1;
	ModelPart m_leg2;
	ModelPart m_leg3;
	ModelPart m_leg4;
	ModelPart m_rightEar;
	ModelPart m_leftEar;
	ModelPart m_snout;
	ModelPart m_tail;
	ModelPart m_mane;
	float m_yBabyHeadOffset;
	float m_zBabyHeadOffset;
};
