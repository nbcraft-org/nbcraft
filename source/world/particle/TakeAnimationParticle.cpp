#include "Particle.hpp"
#include "world/level/Level.hpp"
#include "client/renderer/entity/EntityRenderDispatcher.hpp"
#include "renderer/ShaderConstants.hpp"

TakeAnimationParticle::TakeAnimationParticle(Level* level, Entity* throwed, Entity* thrower, float vel) 
	: Particle(level, throwed->m_pos, throwed->m_vel)
	, m_pThrowed(throwed)
	, m_pThrower(thrower)
	, m_speed(vel)
{
	throwed->m_bCanBeDeleted = false;
	m_lifetime = 3;
}

TakeAnimationParticle::~TakeAnimationParticle()
{
	delete m_pThrowed;
}

void TakeAnimationParticle::render(Tesselator& t, float f, float a, float b, float c, float d, float e)
{
	float p = (m_age + f) / m_lifetime;
	p *= p;
	Vec3 tPos(
		Mth::Lerp(m_pThrower->m_posPrev.x, m_pThrower->m_pos.x, f),
		Mth::Lerp(m_pThrower->m_posPrev.y, m_pThrower->m_pos.y, f) + m_speed,
		Mth::Lerp(m_pThrower->m_posPrev.z, m_pThrower->m_pos.z, f)
	);
	Vec3 pos = m_pos + (tPos - m_pos) * p;
	float bright = m_pLevel->getBrightness(TilePos(pos.x, pos.y + (m_heightOffset / 2.0f), pos.z));
	currentShaderDarkColor = Color(bright, bright, bright);
	EntityRenderDispatcher::getInstance()->render(*m_pThrowed, Vec3(pos.x - xOff, pos.y - yOff, pos.z - zOff), m_pThrowed->m_rot.y, f);
}

void TakeAnimationParticle::tick()
{
	m_age++;
	if (m_age >= m_lifetime)
		remove();

}

int TakeAnimationParticle::getParticleTexture()
{
	return PT_PARTICLES3;
}

