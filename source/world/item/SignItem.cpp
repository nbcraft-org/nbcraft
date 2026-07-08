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
    else if (!level->getMaterial(pos)->isSolid())
        return false;
    else
    {
        TilePos tp = pos.relative(face);

        if (!Tile::sign->mayPlace(level, tp))
            return false;
        else
        {
            if (face == Facing::UP)
                level->setTileAndData(tp, Tile::sign->m_ID, Mth::floor(((player->m_rot.yaw + 180.0f) * 16.0f / 360.0f) + 0.5) & 15);
            else
                level->setTileAndData(tp, Tile::wallSign->m_ID, face);

            --stack->m_count;
            SignTileEntity* sign = static_cast<SignTileEntity*>(level->getTileEntity(tp));
            if (sign)
                player->openTextEdit(sign);

            return true;
        }
    }
}
