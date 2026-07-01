#include "world/level/Level.hpp"
#include "LightningBolt.hpp"


void LightningBolt::_init()
{
	m_pDescriptor = &EntityTypeDescriptor::lightningBolt;
	m_renderType = RENDER_LIGHTNING_BOLT;
	m_life = 2;
	m_seed = m_random.nextInt();
	m_flashes = m_random.nextInt(3) + 1;
	if (m_pLevel->m_difficulty >= 2 && m_pLevel->hasChunksAt(m_pos, 10))
	{
		TilePos tp(m_pos);
		if (m_pLevel->getTile(tp) == 0 && Tile::fire->mayPlace(m_pLevel, tp))
			m_pLevel->setTile(tp, Tile::fire->m_ID);

		for (int i = 0; i < 4; ++i)
		{
			tp.x = Mth::floor(m_pos.x) + m_random.nextInt(3) - 1;
			tp.y = Mth::floor(m_pos.y) + m_random.nextInt(3) - 1;
			tp.z = Mth::floor(m_pos.z) + m_random.nextInt(3) - 1;
			if (m_pLevel->getTile(tp) == 0 && Tile::fire->mayPlace(m_pLevel, tp))
				m_pLevel->setTile(tp, Tile::fire->m_ID);
		}
	}
}

LightningBolt::LightningBolt(Level* level) 
	: Entity(level)
{
	_init();
}

LightningBolt::LightningBolt(Level* level, const Vec3& pos) 
	: LightningBolt(level)
{
	_init();
	moveTo(pos);
}

void LightningBolt::tick()
{
	Entity::tick();
	if (m_life == 2)
	{
		m_pLevel->playSound(m_pos, "ambient.weather.thunder", 10000.0F, 0.8F + m_random.nextFloat() * 0.2F);
		m_pLevel->playSound(m_pos, "random.explode", 2.0F, 0.5F + m_random.nextFloat() * 0.2F);
	}

	--m_life;
	if (m_life < 0)
	{
		if (m_flashes == 0)
			remove();
		else if (m_life < -m_random.nextInt(10))
		{
			--m_flashes;
			m_life = 1;
			m_seed = m_random.nextLong();
			if (m_pLevel->hasChunksAt(m_pos, 10))
			{
				TilePos tp(m_pos);
				if (m_pLevel->getTile(tp) == 0 && Tile::fire->mayPlace(m_pLevel, tp))
					m_pLevel->setTile(tp, Tile::fire->m_ID);
			}
		}
	}

	if (m_life >= 0)
	{
		constexpr float var6 = 3.0;
		EntityVector ents = m_pLevel->getEntities(this, AABB(m_pos - var6, m_pos + var6));

		if (!ents.empty())
		{
			for (EntityVector::iterator it = ents.begin(); it != ents.end(); ++it)
			{
				(*it)->thunderHit(this);
			}
		}

		m_pLevel->m_skyFlashTime = 2;
	}
}

bool LightningBolt::shouldRender(Vec3& camPos) const
{
	return m_life >= 0;
}
