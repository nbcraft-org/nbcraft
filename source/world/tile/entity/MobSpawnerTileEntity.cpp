#include "MobSpawnerTileEntity.hpp"
#include "world/entity/MobFactory.hpp"
#include "world/level/Level.hpp"

const int MobSpawnerTileEntity::MAX_DIST = 16;

MobSpawnerTileEntity::MobSpawnerTileEntity()
    : m_spawnDelay(20)
    , m_pEntityDescriptor(&EntityTypeDescriptor::pig)
    , m_spin(0)
    , m_oSpin(0)
{
    m_pType = TileEntityType::mobSpawner;
}

bool MobSpawnerTileEntity::isNearPlayer() const
{
    return m_pLevel && m_pLevel->getNearestPlayer(m_pos + 0.5f, (float)MAX_DIST, false);
}

void MobSpawnerTileEntity::tick()
{
    m_oSpin = m_spin;
    if (!m_pLevel || !isNearPlayer()) return;

    Vec3 particlePos(
        m_pos.x + m_pLevel->m_random.nextFloat(),
        m_pos.y + m_pLevel->m_random.nextFloat(),
        m_pos.z + m_pLevel->m_random.nextFloat()
    );

    m_pLevel->addParticle("smoke", particlePos);
    m_pLevel->addParticle("flame", particlePos);

    m_spin += (1000.0f / (m_spawnDelay + 200.0f));
    while (m_spin > 360.0f)
    {
        m_spin -= 360.0f;
        m_oSpin -= 360.0f;
    }

    if (m_spawnDelay == -1)
        delay();

    if (m_spawnDelay > 0)
    {
        --m_spawnDelay;
        return;
    }

    for (int i = 0; i < C_SPAWNER_MAX_SPAWN; ++i)
    {
        Mob* mob = MobFactory::CreateMob(m_pEntityDescriptor->getEntityType().getId(), m_pLevel);
        if (!mob) return;

        AABB box(m_pos.x, m_pos.y, m_pos.z, m_pos.x, m_pos.y, m_pos.z);
        box.grow(8.0f, 4.0f, 8.0f);
        int existing = m_pLevel->getEntitiesOfCategory(mob->getDescriptor().getCategories().getCategoryMask(), box).size();

        if (existing >= C_SPAWNER_MAX_MOBS)
        {
            delay();
            return;
        }

        Vec3 spawnPos(
            float(m_pos.x) + (m_pLevel->m_random.nextFloat() - m_pLevel->m_random.nextFloat()) * 4.0f,
            float(m_pos.y) + m_pLevel->m_random.nextInt(3) - 1,
            float(m_pos.z) + (m_pLevel->m_random.nextFloat() - m_pLevel->m_random.nextFloat()) * 4.0f
        );

        mob->moveTo(spawnPos, Rot2(m_pLevel->m_random.nextFloat() * 360.0f, 0.0f));

        if (mob->canSpawn())
        {
            m_pLevel->addEntity(mob);

            for (int j = 0; j < 20; ++j)
            {
                Vec3 particleSpawn(
                    m_pos.x + 0.5f + (m_pLevel->m_random.nextFloat() - 0.5f) * 2.0f,
                    m_pos.y + 0.5f + (m_pLevel->m_random.nextFloat() - 0.5f) * 2.0f,
                    m_pos.z + 0.5f + (m_pLevel->m_random.nextFloat() - 0.5f) * 2.0f
                );
                m_pLevel->addParticle("smoke", particleSpawn, Vec3::ZERO);
                m_pLevel->addParticle("flame", particleSpawn, Vec3::ZERO);
            }

            mob->spawnAnim();
            delay();
        }
    }

    TileEntity::tick();
}

void MobSpawnerTileEntity::delay()
{
    if (m_pLevel)
        m_spawnDelay = 200 + m_pLevel->m_random.nextInt(600);
}

void MobSpawnerTileEntity::load(const CompoundTag& tag)
{
    TileEntity::load(tag);
    setEntityId(tag.getString("EntityId"));
    m_spawnDelay = tag.getInt16("Delay");
}

void MobSpawnerTileEntity::save(CompoundTag& tag) const
{
    TileEntity::save(tag);
    tag.putString("EntityId", getEntityId());
    tag.putInt16("Delay", m_spawnDelay);
}
