#include "SignModel.hpp"

SignModel::SignModel() 
	: Model(64, 32)
	, m_cube(this, 0, 0)
	, m_cube2(this, 0, 14)
{
	m_cube.addBox(-12.0f, -14.0f, -1.0f, 24, 12, 2, 0.0f);
	m_cube2.addBox(-1.0f, -2.0f, -1.0f, 2, 14, 2, 0.0f);
}

SignModel::~SignModel()
{
}

void SignModel::render(float time, float r, float bob, float yRot, float xRot, float scale)
{
	m_cube.render(0.0625f);
	m_cube2.render(0.0625f);
}
