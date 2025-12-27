#include "PigZombie.hpp"
#include "nbt/CompoundTag.hpp"

ItemInstance* PigZombie::m_sword = nullptr;

PigZombie::PigZombie(Level* pLevel) : Zombie(pLevel)
{
	m_pDescriptor = &EntityTypeDescriptor::pigZombie;
	m_texture = "mob/pigzombie.png";
	m_runSpeed = 0.5f;
	m_attackDamage = 5;
	m_bFireImmune = true;
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
	return m_pLevel->m_difficulty > 0 && m_pLevel->isUnobstructed(&m_hitbox) && m_pLevel->getCubes(this, m_hitbox)->empty() && !m_pLevel->containsAnyLiquid(m_hitbox);
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
	Zombie::aiStep();
}

bool PigZombie::hurt(Entity* pCulprit, int damage)
{
	if (pCulprit && pCulprit->isPlayer()) {
		AABB bb = m_hitbox;
		bb.grow(32.0f, 32.0f, 32.0f);
		EntityVector entities = m_pLevel->getEntities(this, bb);

		for (Entity* entity : entities) {
			if (entity->getDescriptor().isType(EntityType::PIG_ZOMBIE)) {
				static_cast<PigZombie*>(entity)->alert(pCulprit);
			}
		}

		alert(pCulprit);
	}

	return Zombie::hurt(pCulprit, damage);
}

void PigZombie::alert(Entity* ent)
{
	m_pAttackTarget = ent;
	m_angerTime = 400 + m_random.nextInt(400);
	m_playAngrySoundIn = m_random.nextInt(40);
}

ItemInstance* PigZombie::getCarriedItem() const
{
	if (!m_sword) m_sword = new ItemInstance(Item::sword_gold, 1);
	return m_sword;
}