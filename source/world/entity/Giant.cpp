#include "Giant.hpp"
#include "world/level/TileSource.hpp"

Giant::Giant(TileSource& source) : Monster(source)
{
	m_pDescriptor = &EntityTypeDescriptor::giant;
	m_renderType = RENDER_GIANT;
	m_texture = "mob/zombie.png";
	m_runSpeed = 0.5f;
	m_attackDamage = 50;
	m_health *= 10;
	m_heightOffset *= 6.0f;
	setSize(m_bbWidth * 6.0f, m_bbHeight * 6.0f);
}

float Giant::getWalkTargetValue(const TilePos& pos) const
{
	return m_tileSource->getBrightness(pos) - 0.5f;
}
