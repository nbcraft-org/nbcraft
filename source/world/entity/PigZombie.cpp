#include "PigZombie.hpp"
#include "nbt/CompoundTag.hpp"
#include "world/level/TileSource.hpp"

PigZombie::PigZombie(TileSource& source) : Zombie(source)
{
	m_pDescriptor = &EntityTypeDescriptor::pigZombie;
	m_texture = "mob/pigzombie.png";
	m_runSpeed = 0.5f;
	m_attackDamage = 5;
	m_bFireImmune = true;
	m_angerTime = 0;
	m_playAngrySoundIn = 0;
}

void PigZombie::tick()
{
	m_runSpeed = m_pAttackTarget != nullptr ? 0.95f : 0.5f;
	if (m_playAngrySoundIn > 0 && --m_playAngrySoundIn == 0)
	{
		m_pLevel->playSound(this, "mob.zombiepig.zpigangry", getSoundVolume() * 2.0f, ((m_random.nextFloat() - m_random.nextFloat()) * 0.2f + 1.0f) * 1.8f);
	}

	Zombie::tick();
}

bool PigZombie::canSpawn()
{
	return m_pLevel->m_difficulty > 0 && m_tileSource->isUnobstructedByEntities(m_hitbox, this) && m_tileSource->fetchAABBs(m_hitbox).empty() && !m_tileSource->containsAnyLiquid(m_hitbox);
}

void PigZombie::addAdditionalSaveData(CompoundTag& tag) const
{
	Zombie::addAdditionalSaveData(tag);
	tag.putInt16("Anger", m_angerTime); // putShort()
}

void PigZombie::readAdditionalSaveData(const CompoundTag& tag)
{
	Zombie::readAdditionalSaveData(tag);
	m_angerTime = tag.getInt16("Anger");
}

Entity* PigZombie::findAttackTarget()
{
	return m_angerTime == 0 ? nullptr : Zombie::findAttackTarget();
}

void PigZombie::aiStep()
{
	Monster::aiStep();
}

bool PigZombie::hurt(Entity* pCulprit, int damage)
{
	if (pCulprit && pCulprit->isPlayer())
	{
		AABB bb = m_hitbox;
		// @PARITY-PE: 16x16x16 on PE
		bb.grow(32.0f, 32.0f, 32.0f);
		Entity::Vector entities = m_tileSource->getEntities(this, bb);

		for (size_t i = 0; i < entities.size(); i++)
		{
			if (!entities[i]->getDescriptor().isType(EntityType::PIG_ZOMBIE))
				continue;

			PigZombie* pigZombie = static_cast<PigZombie*>(entities[i]);
			pigZombie->_alert(pCulprit);
		}

		_alert(pCulprit);
	}

	return Zombie::hurt(pCulprit, damage);
}

void PigZombie::_alert(Entity* ent)
{
	m_pAttackTarget = ent;
	m_angerTime = 400 + m_random.nextInt(400);
	m_playAngrySoundIn = m_random.nextInt(40);
}

ItemStack& PigZombie::getCarriedItem() const
{
	static ItemStack sword = ItemStack(Item::sword_gold, 1);
	return sword;
}
