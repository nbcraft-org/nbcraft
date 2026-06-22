#pragma once

#include "Model.hpp"

class SlimeModel : public Model
{
protected:
	struct Materials
	{
		mce::MaterialPtr slime_outer;

		Materials();
	};

public:
	SlimeModel(int);
	~SlimeModel();
	void setupAnim(float, float, float, float, float, float) override;
	void render(float, float, float, float, float, float) override;

private:
	ModelPart m_cube;
	ModelPart m_eye0;
	ModelPart m_eye1;
	ModelPart m_mouth;
	int m_vOffs;

protected:
	Materials m_slimeMaterials;

};
