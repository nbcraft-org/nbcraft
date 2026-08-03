#pragma once

#include "Entity.hpp"

class LightningBolt : public Entity
{
private:
	void _init();
	int m_life;
	int m_flashes;

public:
	LightningBolt(Level* level);
	LightningBolt(Level* level, const Vec3& pos);

	void tick() override;
	virtual bool shouldRender(Vec3& camPos) const override;

	int32_t m_seed;
};

