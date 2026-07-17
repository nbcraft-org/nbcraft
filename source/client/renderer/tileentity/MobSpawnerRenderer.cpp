#include "MobSpawnerRenderer.hpp"
#include "../entity/EntityRenderDispatcher.hpp"
#include "world/entity/MobFactory.hpp"

void MobSpawnerRenderer::renderTyped(MobSpawnerTileEntity* spawner, const Vec3& pos, float partialTicks)
{
    MatrixStack::Ref mtx = MatrixStack::World.push();
    mtx->translate(Vec3(pos.x + 0.5f, pos.y, pos.z + 0.5f));

    EntityType::ID entityId = spawner->m_pEntityDescriptor->getEntityType().getId();

    HashMap<EntityType::ID, Entity*> ::iterator it = m_models.find(entityId);
    Entity* e = nullptr;

    if (it == m_models.end())
    {
        e = MobFactory::CreateMob(entityId, nullptr);
        if (e)
            m_models[entityId] = e;
    } 
    else
        e = it->second;

    if (e)
    {
        e->setLevel(spawner->m_pLevel);

        constexpr float scale = 0.4375f;

        mtx->translate(Vec3(0.0f, 0.4f, 0.0f));
        float spin = float(spawner->m_oSpin + (spawner->m_spin - spawner->m_oSpin) * partialTicks) * 10.0f;
        mtx->rotate(spin, Vec3::UNIT_Y);
        mtx->rotate(-30.0f, Vec3::UNIT_X);
        mtx->translate(Vec3(0.0f, -0.4f, 0.0f));

        mtx->scale(scale);

        e->moveTo(spawner->m_pos, Rot2::ZERO);
        EntityRenderDispatcher::getInstance()->render(*e, Vec3::ZERO, 0.0f, partialTicks);
    }

}
