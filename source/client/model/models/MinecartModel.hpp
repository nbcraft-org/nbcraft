#pragma once

#include "Model.hpp"

class MinecartModel : public Model
{
public:
	MinecartModel();

	void onGraphicsReset() override;
	void render(float, float, float, float, float, float) override;

public:
	ModelPart m_outerFloor,
		m_frontBarrier,
		m_backBarrier,
		m_leftBarrier,
		m_rightBarrier,
		m_innerFloor;
};
