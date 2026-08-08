#pragma once

#include "Entity.hpp"

class LightningBolt : public Entity
{
public:
	LightningBolt(Level* level);
	LightningBolt(Level* level, const Vec3& pos);

private:
	void _init();

public:
	void tick() override;
	virtual bool shouldRender(Vec3& camPos) const override;

private:
	int m_life;
	int m_flashes;

public:
	int32_t m_seed;
};

