#include "BucketItem.hpp"
#include "world/entity/Player.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"

BucketItem::BucketItem(int id, TileID content) : Item(id)
{
	m_maxStackSize = 1;
    m_maxDamage = 64;
    m_content = content;
}

bool BucketItem::use(ItemStack& item, Mob& user) const
{
    if (!user.isPlayer()) return false;

    Level& level = user.getLevel();
    TileSource& tileSource = user.getTileSource();
    Dimension& dimension = user.getDimension();

    Player* player = static_cast<Player*>(&user);
    Vec3 headPos(user.m_pos.x, user.m_pos.y + 1.62f - user.m_heightOffset, user.m_pos.z);
    constexpr float r = M_PI / 180;
    float var14 = Mth::cos(-player->m_rot.yaw * r - M_PI);
    float var15 = Mth::sin(-player->m_rot.yaw * r - M_PI);
    float var16 = -Mth::cos(-player->m_rot.pitch * r);
    float var17 = Mth::sin(-player->m_rot.pitch * r);
    float var18 = var15 * var16;
    float var20 = var14 * var16;
    HitResult hitResult = tileSource.clip(headPos, headPos.add(var18 * 5, var17 * 5, var20 * 5), m_content == TILE_AIR);
    if (!hitResult.isHit())
        return false;
    else
    {
        if (hitResult.m_hitType == HitResult::TILE)
        {
            TilePos tp(hitResult.m_tilePos);
            if (!tileSource.mayInteract(player, tp))
                return false;

            Material* material = tileSource.getMaterial(tp);
            TileData data = tileSource.getData(tp);

            if (m_content == TILE_AIR)
            {
                if (material == Material::water && data == 0)
                {
                    tileSource.setTile(tp, TILE_AIR);
                    player->m_pInventory->setSelectedItem(Item::bucket_water);
                    return true;
                }
                else if (material == Material::lava && data == 0)
                {
                    tileSource.setTile(tp, TILE_AIR);
                    player->m_pInventory->setSelectedItem(Item::bucket_lava);
                    return true;
                }
            }
            else
            {
                tp = tp.relative(hitResult.m_hitSide);

                if (tileSource.isEmptyTile(tp) || !tileSource.getMaterial(tp)->isSolid())
                {
                    if (m_content != TILE_PISTON_MOVING) // milk
                    {
                        if (dimension.isUltraWarm() && m_content == Tile::water->m_ID)
                        {
                            level.playSound(headPos + 0.5f, "random.fizz", 0.5f, 2.6f + (level.m_random.nextFloat() - level.m_random.nextFloat()) * 0.8f);

                            for (int i = 0; i < 8; ++i)
                                level.addParticle("largesmoke", Vec3(tp.x + Mth::random(), tp.y + Mth::random(), tp.z + Mth::random()));
                        }
                        else
                            tileSource.setTileAndData(tp, FullTile(m_content, 0));
                    }

                    if (!player->isCreative())
                        player->m_pInventory->setSelectedItem(Item::bucket_empty);
                    return true;
                }
            }
        }

        return false;
    }
}
