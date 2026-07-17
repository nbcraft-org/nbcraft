#include "SlimeModel.hpp"

SlimeModel::SlimeModel(int vOffs) :
	Model(64, 32),
	m_cube(this, 0, vOffs),
	m_eye0(32, 0),
	m_eye1(32, 4),
	m_mouth(32, 8)
{
	m_vOffs = vOffs;
	m_pMaterial = &m_materials.entity_alphablend;

	if (vOffs > 0)
	{
		m_cube.addBox(-3.0f, 17.0f, -3.0f, 6, 6, 6);
		m_eye0.setModel(this);
		m_eye0.addBox(-3.25f, 18.0f, -3.5f, 2, 2, 2);
		m_eye1.setModel(this);
		m_eye1.addBox(1.25f, 18.0f, -3.5f, 2, 2, 2);
		m_mouth.setModel(this);
		m_mouth.addBox(0.0f, 21.0f, -3.5f, 1, 1, 1);
	}
	else
	{
		m_cube.addBox(-4.0f, 16.0f, -4.0f, 8, 8, 8);
	}
}

SlimeModel::~SlimeModel()
{
}

void SlimeModel::render(float a, float b, float c, float d, float e, float f)
{
	setupAnim(a, b, c, d, e, f);
	m_cube.render(f);
	if (m_vOffs > 0)
	{
		m_eye0.render(f);
		m_eye1.render(f);
		m_mouth.render(f);
	}
}

void SlimeModel::setupAnim(float a, float b, float c, float d, float e, float f)
{
}