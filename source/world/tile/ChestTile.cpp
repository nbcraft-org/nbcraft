#include "ChestTile.hpp"
#include "world/inventory/CompoundContainer.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"
#include "world/tile/entity/ChestTileEntity.hpp"

ChestTile::ChestTile(int id, int texture) : EntityTile(id, texture, Material::wood)
{
    setTicking(true);
}

int ChestTile::getTexture(TileSource& source, const TilePos& pos, Facing::Name face) const
{
    if (face == Facing::UP || face == Facing::DOWN)
        return m_TextureFrame - 1;

    TileID id_north = source.getTile(pos.north());
    TileID id_south = source.getTile(pos.south());
    TileID id_west = source.getTile(pos.west());
    TileID id_east = source.getTile(pos.east());

    bool isDoubleNS = (id_north == m_ID || id_south == m_ID);
    bool isDoubleWE = (id_west == m_ID || id_east == m_ID);

    if (!isDoubleNS && !isDoubleWE)
    {
        Facing::Name front = Facing::SOUTH;

        if (Tile::solid[id_north] && !Tile::solid[id_south])
            front = Facing::SOUTH;
        else if (Tile::solid[id_south] && !Tile::solid[id_north])
            front = Facing::NORTH;
        else if (Tile::solid[id_west] && !Tile::solid[id_east])
            front = Facing::EAST;
        else if (Tile::solid[id_east] && !Tile::solid[id_west])
            front = Facing::WEST;

        return (face == front) ? m_TextureFrame + 1 : m_TextureFrame;
    }

    bool left = false;
    Facing::Name front = Facing::SOUTH;

    if (isDoubleWE)
    {
        left = id_west == m_ID;
        TilePos side = left ? pos.west() : pos.east();

        TileID id_behind = source.getTile(side.north());
        TileID id_infront = source.getTile(side.south());

        if ((Tile::solid[id_north] || Tile::solid[id_behind]) && !Tile::solid[id_south] && !Tile::solid[id_infront])
            front = Facing::SOUTH;
        else if ((Tile::solid[id_south] || Tile::solid[id_infront]) && !Tile::solid[id_north] && !Tile::solid[id_behind])
            front = Facing::NORTH;

        if (front == Facing::SOUTH)
            left = !left;
        if (face == front)
            return m_TextureFrame + (left ? 15 : 16);
        if (face == Facing::OPPOSITE[front])
            return m_TextureFrame + (left ? 32 : 31);
        return m_TextureFrame;
    }

    if (isDoubleNS)
    {
        front = Facing::EAST;
        left = id_north == m_ID;
        TilePos side = left ? pos.north() : pos.south();

        TileID id_behind = source.getTile(side.west());
        TileID id_infront = source.getTile(side.east());

        if ((Tile::solid[id_west] || Tile::solid[id_behind]) && !Tile::solid[id_east] && !Tile::solid[id_infront])
            front = Facing::EAST;
        else if ((Tile::solid[id_east] || Tile::solid[id_infront]) && !Tile::solid[id_west] && !Tile::solid[id_behind])
        {
            front = Facing::WEST;
            left = !left;
        }

        if (face == front)
            return m_TextureFrame + (left ? 15 : 16);
        if (face == Facing::OPPOSITE[front])
            return m_TextureFrame + (left ? 32 : 31);
        return m_TextureFrame;
    }

    return m_TextureFrame;
}

int ChestTile::getTexture(Facing::Name face) const
{
	switch (face)
    {
	case Facing::UP:
    case Facing::DOWN:
        return m_TextureFrame - 1;
	case Facing::SOUTH:
        return m_TextureFrame + 1;
	default:
        return m_TextureFrame;
	}
}

bool ChestTile::mayPlace(const TileSource& source, const TilePos& pos) const
{
    return !hasNeighbors(source, pos, 1);
}

bool ChestTile::hasNeighbors(const TileSource& source, const TilePos& pos, int count) const
{
    int neighbors = 0;
    for (int i = 0; i < 4; ++i)
    {
        Facing::Name f = static_cast<Facing::Name>(Facing::HORIZONTAL[i]);
        TilePos relative = pos.relative(f);
        if (source.getTile(relative) == m_ID)
        {
            neighbors++;
            if (neighbors > count) return true;

            if (hasNeighbors(source, relative, 0)) return true;
        }
    }

    return false;
}

void ChestTile::onRemove(TileSource& source, const TilePos& pos)
{
    Level& level = source.getLevel();
    ChestTileEntity* ent = static_cast<ChestTileEntity*>(source.getTileEntity(pos));

    if (!ent)
    {
        EntityTile::onRemove(source, pos);
        return;
    }

    for (Container::StackID slot = 0; slot < ent->getContainerSize(); ++slot)
    {
        ItemStack& item = ent->getItem(slot);
        if (item.isEmpty())
            continue;

        constexpr float spread = 0.05f;

        TilePos offset(
            (m_chestRandom.nextFloat() * 0.8f) + 0.1f,
            (m_chestRandom.nextFloat() * 0.8f) + 0.1f,
            (m_chestRandom.nextFloat() * 0.8f) + 0.1f
        );

        while (item.m_count > 0)
        {
            int toRemove = std::min(m_chestRandom.nextInt(21) + 10, static_cast<int>(item.m_count));
            item.m_count -= toRemove;
            ItemEntity* itemEnt = new ItemEntity(source, pos + offset, ItemStack(item.getItem()->m_itemID, toRemove, item.getAuxValue()));
            itemEnt->m_vel.x = (double)((float)m_chestRandom.nextGaussian() * spread);
            itemEnt->m_vel.y = (double)((float)m_chestRandom.nextGaussian() * spread + 0.2f);
            itemEnt->m_vel.z = (double)((float)m_chestRandom.nextGaussian() * spread);
            level.addEntity(itemEnt);
        }
    }

    EntityTile::onRemove(source, pos);
}

bool ChestTile::use(const TilePos& pos, Player& player)
{
	if (player.isSneaking() && !player.getSelectedItem().isEmpty())
		return false;

    Level& level = player.getLevel();
    TileSource& source = player.getTileSource();

    if (level.m_bIsClientSide)
        return true;

    if (source.isSolidBlockingTile(pos.above()))
        return true;
    
    if (source.getTile(pos.west()) == m_ID && source.isSolidBlockingTile(pos.west().above()))
        return true;

    if (source.getTile(pos.east()) == m_ID && source.isSolidBlockingTile(pos.east().above()))
        return true;

    if (source.getTile(pos.north()) == m_ID && source.isSolidBlockingTile(pos.north().above()))
        return true;

    if (source.getTile(pos.south()) == m_ID && source.isSolidBlockingTile(pos.south().above()))
        return true;

    TileEntity* tileEnt = source.getTileEntity(pos);
    if (!tileEnt)
        return false;

    ChestTileEntity* chest = (ChestTileEntity*)tileEnt;
    Container* container = dynamic_cast<Container*>(chest);
    for (int rel = Facing::NORTH; rel <= Facing::EAST; rel++) 
    {
        TilePos relPos = pos.relative((Facing::Name)rel);
        if (source.getTile(pos.relative((Facing::Name)rel)) == m_ID)
        {
            TileEntity* nearbyTileEntity = source.getTileEntity(relPos);
            if (!nearbyTileEntity)
                continue;

            Container* nearbyContainer = dynamic_cast<Container*>(nearbyTileEntity);
            if (rel % 2 == 0)
                container = new CompoundContainer("Large chest", nearbyContainer, container);
            else
                container = new CompoundContainer("Large chest", container, nearbyContainer);
            break;
        }
    }

    player.openContainer(container);
    return true;
}

bool ChestTile::hasTileEntity() const
{
    return true;
}

TileEntity* ChestTile::newTileEntity()
{
    return new ChestTileEntity();
}
