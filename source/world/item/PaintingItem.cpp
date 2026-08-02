#include "PaintingItem.hpp"
#include "world/level/Level.hpp"
#include <world/entity/Painting.hpp>

PaintingItem::PaintingItem(int id) : Item(id)
{
}

bool PaintingItem::useOn(ItemStack* instance, Player* player, Level* level, const TilePos& pos, Facing::Name face) const
{
    if (Facing::isVertical(face))
        return false;

    uint8_t var8 = 0;
    switch (face)
    {
    case Facing::WEST:
        var8 = 1;
        break;
    case Facing::SOUTH:
        var8 = 2;
        break;
    case Facing::EAST:
        var8 = 3;
        break;
    }

    Painting* painting = new Painting(level, pos, var8);
        if (painting->survives())
        {
            if (!level->m_bIsClientSide)
                level->addEntity(painting);

            --instance->m_count;
        }
    else
        delete painting;

    return true;
}
