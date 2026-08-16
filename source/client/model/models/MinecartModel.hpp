#pragma once

#include "Model.hpp"

class MinecartModel : public Model
{
public:
	MinecartModel();
	~MinecartModel();

	void render(float, float, float, float, float, float) override;

public:
	ModelPart* m_cubes[6];
};
