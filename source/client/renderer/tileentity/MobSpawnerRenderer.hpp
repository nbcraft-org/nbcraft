#pragma once

#include "TileEntityRenderer.hpp"
#include "world/tile/entity/MobSpawnerTileEntity.hpp"

class MobSpawnerRenderer : public TileEntityRenderer<MobSpawnerTileEntity>
{
public:
    void renderTyped(MobSpawnerTileEntity* spawner, const Vec3&, float partialTicks) override;

private:
    HashMap<EntityType::ID, Entity*> m_models;
};
