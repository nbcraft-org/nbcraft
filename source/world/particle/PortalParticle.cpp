/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Particle.hpp"

PortalParticle::PortalParticle(Level* level, const Vec3& pos, const Vec3& dir) :
	Particle(level, pos, dir)
{
	m_vel = dir;
	m_startPos = m_pos = pos;
	float br = m_random.nextFloat() * 0.6f + 0.4f;
	m_oSize = m_size = m_random.nextFloat() * 0.2f + 0.5f;
	m_rCol = m_gCol = m_bCol = 1.0f * br;
	m_gCol *= 0.3f;
	m_rCol *= 0.9f;
	m_lifetime = (int)(Mth::random() * 10.0) + 40;
	m_bNoPhysics = true;
	m_tex = Mth::random() * 8.0;
}

float PortalParticle::getBrightness(float a) const
{
	float br = Particle::getBrightness(a);
	float pos = (float)m_age / (float)m_lifetime;
	pos *= pos;
	pos *= pos;
	return br * (1.0f - pos) + pos;
}

void PortalParticle::tick()
{
	m_oPos = m_pos;
	float pos = (float)m_age / (float)m_lifetime;
	float a = pos;
	pos = -pos + pos * pos * 2.0f;
	pos = 1.0f - pos;
	m_pos.x = m_startPos.x + m_vel.x * pos;
	m_pos.y = m_startPos.y + m_vel.y * pos + (1.0f - a);
	m_pos.z = m_startPos.z + m_vel.z * pos;
	if (m_age++ >= m_lifetime)
		remove();
}

void PortalParticle::render(Tesselator& t, float f, float a, float b, float c, float d, float e)
{
	float s = ((float)m_age + a) / (float)m_lifetime;
	s = 1.0f - s;
	s *= s;
	s = 1.0f - s;
	m_size = m_oSize * s;
	Particle::render(t, f, a, b, c, d, e);
}
