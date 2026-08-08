#include "SignItem.hpp"
#include "world/level/Level.hpp"
#include "world/entity/Player.hpp"
#include "world/tile/Tile.hpp"
#include "world/tile/entity/SignTileEntity.hpp"

SignItem::SignItem(int id) : Item(id)
{
	m_maxStackSize = 1;
}

bool SignItem::useOn(ItemStack* stack, Player* player, Level* level, const TilePos& pos, Facing::Name face) const
{
    if (face == Facing::DOWN)
        return false;

    if (!level->getMaterial(pos)->isSolid())
        return false;

    TilePos tp = pos.relative(face);

    if (!Tile::sign->mayPlace(level, tp))
        return false;

    TileID tile = (face == Facing::UP) ? Tile::sign->m_ID : Tile::wallSign->m_ID;
    TileData data = (face == Facing::UP) ? (TileData)(Mth::floor(((player->m_rot.yaw + 180.0f) * 16.0f / 360.0f) + 0.5) & 15) : (TileData)face;
    level->setTileAndData(tp, tile, data);

    --stack->m_count;
    SignTileEntity* sign = dynamic_cast<SignTileEntity*>(level->getTileEntity(tp));
    if (sign)
        player->openTextEdit(sign);

    return true;
}
