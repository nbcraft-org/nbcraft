#pragma once

#include "TileEntityRenderer.hpp"
#include "world/tile/entity/PistonMovingTileEntity.hpp"
#include "../TileRenderer.hpp"

class PistonRenderer : public TileEntityRenderer<PistonMovingTileEntity>
{
public:
    void renderTyped(PistonMovingTileEntity* piston, const Vec3&, float partialTicks) override;

    TileRenderer m_tileRenderer;
};
