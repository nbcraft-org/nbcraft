#include "Slime.hpp"
#include "world/level/Level.hpp"
#include "nbt/CompoundTag.hpp"

Slime::Slime(Level* pLevel) : Mob(pLevel)
{
	m_pDescriptor = &EntityTypeDescriptor::slime;
	m_renderType = RENDER_SLIME;
	m_texture = "mob/slime.png";
	m_size = 1 << m_random.nextInt(3);
	m_heightOffset = 0.0f;
	m_jumpDelay = m_random.nextInt(20) + 10;
	setSize(m_size);
}

void Slime::setSize(int size)
{
	m_size = size;
	Mob::setSize(0.6f * size, 0.6f * size);
	m_health = size * size;
	setPos(m_pos);
}

void Slime::addAdditionalSaveData(CompoundTag& tag) const
{
	Mob::addAdditionalSaveData(tag);
	tag.putInt32("Size", m_size - 1);
}

void Slime::readAdditionalSaveData(const CompoundTag& tag)
{
	Mob::readAdditionalSaveData(tag);
	m_size = tag.getInt32("Size") + 1;
}

void Slime::tick()
{
	m_oSquish = m_squish;
	bool wasOnGround = m_bOnGround;
	Mob::tick();
	if (m_bOnGround && !wasOnGround)
	{
		for (int i = 0; i < m_size * 8; i++)
		{
			float dir = m_random.nextFloat() * float(M_PI) * 2.0f;
			float d = m_random.nextFloat() * 0.5f + 0.5f;
			float xd = Mth::sin(dir) * float(m_size) * 0.5f * d;
			float zd = Mth::cos(dir) * float(m_size) * 0.5f * d;
			m_pLevel->addParticle("slime", Vec3(m_pos.x + xd, m_hitbox.min.y, m_pos.z + zd));
		}

		if (m_size > 2)
		{
			m_pLevel->playSound(this, "mob.slime", getSoundVolume(), ((m_random.nextFloat() - m_random.nextFloat()) * 0.2f + 1.0f) / 0.8f);
		}

		m_squish = -0.5f;
	}

	m_squish *= 0.6f;
}

void Slime::updateAi()
{
	Player* nearestPlayer = m_pLevel->getNearestAttackablePlayer(*this, 16.0f);
	if (nearestPlayer)
	{
		lookAt(nearestPlayer, 10.0f, 10.0f);
	}

	if (m_bOnGround && m_jumpDelay-- <= 0)
	{
		m_jumpDelay = m_random.nextInt(20) + 10;
		if (nearestPlayer)
		{
			m_jumpDelay /= 3;
		}

		m_bJumping = true;
		if (m_size > 1)
		{
			m_pLevel->playSound(this, "mob.slime", getSoundVolume(), ((m_random.nextFloat() - m_random.nextFloat()) * 0.2f + 1.0f) / 0.8f);
		}

		m_squish = 1.0f;
		m_moveVelocity.x = 1.0f - m_random.nextFloat() * 2.0f; // this.xxa
		m_moveVelocity.y = float(1 * m_size); // this.yya
	}
	else
	{
		m_bJumping = false;
		if (m_bOnGround)
		{
			m_moveVelocity.x = m_moveVelocity.y = 0.0f; // this.xxa, this.yya
		}
	}
}

void Slime::remove()
{
	if (m_size > 1 && m_health == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			float xd = (float(i % 2) - 0.5f) * float(m_size) / 4.0f;
			float zd = (float(i / 2) - 0.5f) * float(m_size) / 4.0f;
			Slime* slime = new Slime(m_pLevel);
			slime->setSize(m_size / 2);
			slime->moveTo(Vec3(m_pos.x + xd, m_pos.y + 0.5f, m_pos.z + zd), Vec2(m_random.nextFloat() * 360.0f, 0.0f));
			m_pLevel->addEntity(slime);
		}
	}

	Mob::remove();
}

void Slime::playerTouch(Player* player)
{
	if (m_size > 1 && canSee(player) && distanceTo(player) < 0.6f * float(m_size) && player->hurt(this, m_size))
	{
		m_pLevel->playSound(this, "mob.slimeattack", 1.0f, (m_random.nextFloat() - m_random.nextFloat()) * 0.2f + 1.0f);
	}
}

bool Slime::canSpawn()
{
	LevelChunk* var1 = m_pLevel->getChunkAt(m_pos);
	return (m_size == 1 || m_pLevel->m_difficulty > 0) && m_random.nextInt(10) == 0 && var1->getRandom(987234911).nextInt(10) == 0 && m_pos.y < 16.0f;
}
