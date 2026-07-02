#include "SignModel.hpp"

SignModel::SignModel() 
	: Model(64, 32)
	, m_cube(this, 0, 0)
	, m_cube2(this, 0, 14)
{
	m_cube.addBox(-12.0F, -14.0F, -1.0F, 24, 12, 2, 0.0F);
	m_cube2.addBox(-1.0F, -2.0F, -1.0F, 2, 14, 2, 0.0F);
}

SignModel::~SignModel()
{
}

void SignModel::render(float time, float r, float bob, float yRot, float xRot, float scale)
{
	m_cube.render(0.0625);
	m_cube2.render(0.0625);
}
