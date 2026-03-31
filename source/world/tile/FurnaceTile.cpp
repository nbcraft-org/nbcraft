//
// Created by Dominic Hann on 23/3/2026.
//

#include "FurnaceTile.hpp"
#include "world/entity/Player.hpp"
#include "world/level/Level.hpp"
#include "common/Mth.hpp"

bool FurnaceTile::s_swappingLitState = false;

FurnaceTile::FurnaceTile(TileID id) : Tile(id, TEXTURE_FURNACE_FRONT, Material::stone)
{
}

bool FurnaceTile::use(Level* level, const TilePos& pos, Player* player)
{
    if (player->isSneaking() && !player->getSelectedItem().isEmpty())
    {
        return false;
    }
    if (level->m_bIsClientSide)
    {
        return true;
    }
    else
    {
        player->openFurnace(pos);
        return true;
    }
}

int FurnaceTile::getTexture(const Facing::Name face) const
{
    bool lit = (m_ID == TILE_FURNACE_LIT);
    switch (face) {
        case Facing::UP:    return TEXTURE_FURNACE_TOP;
        case Facing::DOWN:  return Tile::rock->getTexture(face);
        case Facing::SOUTH: return lit ? TEXTURE_FURNACE_LIT : TEXTURE_FURNACE_FRONT;
        default:            return TEXTURE_FURNACE_SIDE;
    }
}

int FurnaceTile::getTexture(Facing::Name face, TileData data) const
{
    bool lit = (m_ID == TILE_FURNACE_LIT);
    int facing = data & 3;
    bool isFront = (face == Facing::SOUTH && facing == 0) ||
                   (face == Facing::WEST  && facing == 1) ||
                   (face == Facing::NORTH && facing == 2) ||
                   (face == Facing::EAST  && facing == 3);
    switch (face) {
        case Facing::UP:   return TEXTURE_FURNACE_TOP;
        case Facing::DOWN: return Tile::rock->getTexture(face);
        default:
            if (isFront) return lit ? TEXTURE_FURNACE_LIT : TEXTURE_FURNACE_FRONT;
            return TEXTURE_FURNACE_SIDE;
    }
}

void FurnaceTile::setPlacedBy(Level* level, const TilePos& pos, Mob* mob)
{
    int rot = Mth::floor(0.5f + (mob->m_rot.x * 4.0f / 360.0f)) & 3;
    TileData data = 0;
    switch (rot) {
        case 0: data = 2; break;
        case 1: data = 3; break;
        case 2: data = 0; break;
        case 3: data = 1; break;
    }
    level->setData(pos, data);
}

void FurnaceTile::onPlace(Level* level, const TilePos& pos) {
    Tile::onPlace(level, pos);
    if (!s_swappingLitState)
        level->setTileEntity(pos.x, pos.y, pos.z, new FurnaceTileEntity(pos));
}

void FurnaceTile::onRemove(Level* level, const TilePos& pos) {
    if (!s_swappingLitState) {
        FurnaceTileEntity* furnace = (FurnaceTileEntity*)level->getTileEntity(pos.x, pos.y, pos.z);
        if (furnace) {
            for (int i = 0; i < furnace->getContainerSize(); i++) {
                ItemStack& item = furnace->getItem(i);
                if (!item.isEmpty()) {
                    Vec3 spawnPos = Vec3(pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f);
                    ItemEntity* entity = new ItemEntity(level, spawnPos, item);
                    entity->m_throwTime = 10;
                    level->addEntity(entity);
                }
            }
        }
        level->removeTileEntity(pos.x, pos.y, pos.z);
    }
    Tile::onRemove(level, pos);
}

int FurnaceTile::getResource(TileData data, Random* pRandom) const
{
    return TILE_FURNACE;
}

TileEntity* FurnaceTile::createTileEntity(const TilePos& pos) {
    return new FurnaceTileEntity(pos);
}