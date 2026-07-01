/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "TripodCamera.hpp"
#include "Player.hpp"
#include "world/level/Level.hpp"

#define C_TIMER 80

TripodCamera::TripodCamera(Entity& owner)
	: Mob(owner.getTileSource())
	, m_owner(owner)
{
	m_countdown = C_TIMER;
	m_bActivated = false;

	m_renderType = RENDER_CAMERA;

	m_oRot = m_rot = owner.m_rot;

    m_bBlocksBuilding = true;

	setSize(1.0f, 1.5f);
	m_heightOffset = m_bbHeight * 0.5f - 0.25f;

	setPos(owner.m_pos);
	m_oPos = owner.m_pos;
	m_bMakeStepSound = false;
}

bool TripodCamera::interact(Player* player)
{
	// @BUG-ish: No check for owner?
	m_bActivated = true;
	return true;
}

bool TripodCamera::interactPreventDefault() const
{
	return true;
}

void TripodCamera::tick()
{
	m_oPos = m_pos;

	m_vel.y -= 0.04f;
	move(m_vel);

	m_vel *= 0.98f;
	if (m_bOnGround)
	{
		m_vel.x *= 0.7f;
		m_vel.z *= 0.7f;
		m_vel.y *= -0.5f;
	}

	if (!m_bActivated)
		return;

	m_countdown--;
	if (m_countdown == 0)
	{
		remove();
	}
	else if (m_countdown == 8)
	{
		m_pLevel->takePicture(this, &m_owner);
		m_pLevel->addParticle("explode", Vec3(m_pos.x, m_pos.y + 0.6f, m_pos.z));
		m_pLevel->addParticle("explode", Vec3(m_pos.x, m_pos.y + 0.8f, m_pos.z));
		m_pLevel->addParticle("explode", Vec3(m_pos.x, m_pos.y + 1.0f, m_pos.z));
	}
	else if (m_countdown > 8)
	{
		m_pLevel->addParticle("smoke", Vec3(m_pos.x, m_pos.y + 1.0f, m_pos.z));
	}
}
