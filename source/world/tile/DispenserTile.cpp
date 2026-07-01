#include "DispenserTile.hpp"
#include "world/entity/Arrow.hpp"
#include "world/entity/Snowball.hpp"
#include "world/entity/ThrownEgg.hpp"
#include "world/tile/entity/DispenserTileEntity.hpp"
#include "world/level/TileSource.hpp"

DispenserTile::DispenserTile(int id) : EntityTile(id, TEXTURE_FURNACE_SIDE, Material::stone)
{
    setTicking(true);
}

void DispenserTile::onPlace(TileSource& source, const TilePos& pos)
{
    EntityTile::onPlace(source, pos);
    recalcLockDir(source, pos);
}

void DispenserTile::recalcLockDir(TileSource& source, const TilePos& pos)
{
    int north = source.getTile(pos.north());
    int south = source.getTile(pos.south());
    int west = source.getTile(pos.west());
    int east = source.getTile(pos.east());
    int data = 3;
    if (Tile::solid[north] && !Tile::solid[south])
    {
        data = 3;
    }

    if (Tile::solid[south] && !Tile::solid[north])
    {
		data = 2;
    }

    if (Tile::solid[west] && !Tile::solid[east])
    {
		data = 5;
    }

    if (Tile::solid[east] && !Tile::solid[west])
    {
		data = 4;
    }

	source.setTileAndData(pos, FullTile(this, data));
}

int DispenserTile::getTexture(TileSource& source, const TilePos& pos, Facing::Name face) const
{
    switch (face)
    {
    case Facing::UP:
        return m_TextureFrame + 17;
    case Facing::DOWN:
        return m_TextureFrame + 17;
    default:
        TileData data = source.getData(pos);
        return face != data ? m_TextureFrame : m_TextureFrame + 1;
    }
}

int DispenserTile::getTexture(Facing::Name face) const
{
    switch (face)
    {
    case Facing::UP:
        return m_TextureFrame + 17;
    case Facing::DOWN:
        return m_TextureFrame + 17;
    default:
        return face == Facing::SOUTH ? m_TextureFrame + 1 : m_TextureFrame;
    }
}

bool DispenserTile::use(const TilePos& pos, Player& player)
{
	if (player.isSneaking() && !player.getSelectedItem().isEmpty())
		return false;

	Level& level = player.getLevel();
	TileSource& source = player.getTileSource();

    if (level.m_bIsClientSide)
        return true;

    DispenserTileEntity* var6 = static_cast<DispenserTileEntity*>(source.getTileEntity(pos));
    player.openTrap(var6);
    return true;
}

void DispenserTile::fireArrow(TileSource& source, const TilePos& pos, Random* random)
{
	Level& level = source.getLevel();

	TileData var6 = source.getData(pos);
	float var9 = 0.0f;
	float var10 = 0.0f;
	switch (var6)
	{
	case 2:
		var10 = -1.0f;
		break;
	case 3:
		var10 = 1.0f;
		break;
	case 5:
		var9 = 1.0f;
		break;
	default:
		var9 = -1.0f;
	}

	DispenserTileEntity* var11 = static_cast<DispenserTileEntity*>(source.getTileEntity(pos));
	ItemStack var12 = var11->removeRandomItem();
	Vec3 exitPos(
		pos.x + var9 * 0.5f + 0.5f,
		pos.y + 0.5f,
		pos.z + var10 * 0.5f + 0.5f
	);

	if (var12.isEmpty())
	{
		level.playSound(pos, "random.click", 1.0f, 1.2f);
		return;
	}

	float var20;
	if (var12.getId() == Item::arrow->m_itemID)
	{
		Arrow* arrow = new Arrow(source, exitPos, true);
		arrow->shoot(var9, 0.1f, var10, 1.1f, 6.0f);
		level.addEntity(arrow);
		level.playSound(pos, "random.bow", 1.0f, 1.2f);
	}
	else if (var12.getId() == Item::egg->m_itemID)
	{
		ThrownEgg* egg = new ThrownEgg(source, exitPos, true);
		egg->shoot(var9, 0.1f, var10, 1.1f, 6.0f);
		level.addEntity(egg);
		level.playSound(pos, "random.bow", 1.0f, 1.2f);
	}
	else if (var12.getId() == Item::snowBall->m_itemID)
	{
		Snowball* snowball = new Snowball(source, exitPos, true);
		snowball->shoot(var9, 0.1f, var10, 1.1f, 6.0f);
		level.addEntity(snowball);
		level.playSound(pos, "random.bow", 1.0f, 1.2f);
	}
	else
	{
		ItemEntity* item = new ItemEntity(source, Vec3(exitPos.x, exitPos.y - 0.3f, exitPos.z), var12);
		var20 = random->nextFloat() * 0.1f + 0.2f;
		item->m_vel.x = var9 * var20;
		item->m_vel.y = 0.2f;
		item->m_vel.z = var10 * var20;
		item->m_vel.x += random->nextGaussian() * 0.0075f * 6.0f;
		item->m_vel.y += random->nextGaussian() * 0.0075f * 6.0f;
		item->m_vel.z += random->nextGaussian() * 0.0075f * 6.0f;
		level.addEntity(item);
		level.playSound(pos, "random.click");
	}

	for (int i = 0; i < 10; i++)
	{
		var20 = random->nextFloat() * 0.2f + 0.01f;

		Vec3 smokePos = exitPos;
		smokePos.x += var9 * 0.01f + (random->nextFloat() - 0.5f) * var10 * 0.5f;
		smokePos.y += (random->nextFloat() - 0.5f) * 0.5f;
		smokePos.z += var10 * 0.01f + (random->nextFloat() - 0.5f) * var9 * 0.5f;

		Vec3 smokeDir(
			var9 * var20 + random->nextGaussian() * 0.01f,
			-0.03f + random->nextGaussian() * 0.01f,
			var10 * var20 + random->nextGaussian() * 0.01f
		);

		level.addParticle("smoke", smokePos, smokeDir);
	}
}

void DispenserTile::neighborChanged(TileSource& source, const TilePos& pos, TileID tile)
{
	if (tile > 0 && Tile::tiles[tile]->isSignalSource())
	{
		bool var6 = source.hasNeighborSignal(pos) || source.hasNeighborSignal(pos.above());
		if (var6)
		{
			source.addToTickNextTick(pos, m_ID, getTickDelay());
		}
	}
}

void DispenserTile::tick(TileSource& source, const TilePos& pos, Random* random)
{
	if (source.hasNeighborSignal(pos) || source.hasNeighborSignal(pos.above()))
	{
		fireArrow(source, pos, random);
	}
}

TileEntity* DispenserTile::newTileEntity()
{
	return new DispenserTileEntity();
}

void DispenserTile::setPlacedBy(const TilePos& pos, Mob& mob)
{
	TileSource& source = mob.getTileSource();
	int rot = Mth::floor(0.5f + (mob.m_rot.yaw * 4.0f / 360.0f)) & 3;

	TileData data = 0;

	switch (rot)
	{
	case 0: data = 2; break;
	case 1: data = 5; break;
	case 2: data = 3; break;
	case 3: data = 4; break;
	}

	source.setTileAndData(pos, FullTile(this, data));
}

bool DispenserTile::hasTileEntity() const
{
    return true;
}
