#pragma once

#include "Model.hpp"

class BoatModel : public Model
{
public:
	BoatModel();
	~BoatModel();

	void render(float, float, float, float, float, float) override;

public:
	ModelPart* m_cubes[5];
};
