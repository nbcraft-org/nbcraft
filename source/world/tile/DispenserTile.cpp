#include "DispenserTile.hpp"
#include "world/level/Level.hpp"
#include "world/tile/entity/DispenserTileEntity.hpp"
#include "world/entity/projectile/Arrow.hpp"
#include "world/entity/projectile/Snowball.hpp"
#include "world/entity/projectile/ThrownEgg.hpp"

DispenserTile::DispenserTile(int id) : EntityTile(id, TEXTURE_FURNACE_SIDE, Material::stone)
{
    setTicking(true);
}

void DispenserTile::onPlace(Level* level, const TilePos& pos)
{
    EntityTile::onPlace(level, pos);
    recalcLockDir(level, pos);
}

void DispenserTile::recalcLockDir(Level* level, const TilePos& pos)
{
    int north = level->getTile(pos.north());
    int south = level->getTile(pos.south());
    int west = level->getTile(pos.west());
    int east = level->getTile(pos.east());
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

    level->setData(pos, data);
}

int DispenserTile::getTexture(const LevelSource* level, const TilePos& pos, Facing::Name face) const
{
    switch (face)
    {
    case Facing::UP:
        return m_TextureFrame + 17;
    case Facing::DOWN:
        return m_TextureFrame + 17;
    default:
        int var6 = level->getData(pos);
        return face != var6 ? m_TextureFrame : m_TextureFrame + 1;
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

bool DispenserTile::use(Level* level, const TilePos& pos, Player* player)
{
	if (player->isSneaking() && !player->getSelectedItem().isEmpty())
		return false;

    if (level->m_bIsClientSide)
        return true;

    DispenserTileEntity* var6 = static_cast<DispenserTileEntity*>(level->getTileEntity(pos));
    player->openTrap(var6);
    return true;
}

void DispenserTile::fireArrow(Level* level, const TilePos& pos, Random* random)
{
	int var6 = level->getData(pos);
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

	DispenserTileEntity* var11 = static_cast<DispenserTileEntity*>(level->getTileEntity(pos));
	ItemStack var12 = var11->removeRandomItem();
	Vec3 exitPos(
		pos.x + var9 * 0.5f + 0.5f,
		pos.y + 0.5f,
		pos.z + var10 * 0.5f + 0.5f
	);

	if (var12.isEmpty())
	{
		level->playSound(pos, "random.click", 1.0f, 1.2f);
		return;
	}

	float var20;
	if (var12.getId() == Item::arrow->m_itemID)
	{
		Arrow* var19 = new Arrow(level, exitPos, true);
		var19->shoot(var9, 0.1f, var10, 1.1f, 6.0f);
		level->addEntity(var19);
		level->playSound(pos, "random.bow", 1.0f, 1.2f);
	}
	else if (var12.getId() == Item::egg->m_itemID)
	{
		ThrownEgg* var34 = new ThrownEgg(level, exitPos, true);
		var34->shoot(var9, 0.1f, var10, 1.1f, 6.0f);
		level->addEntity(var34);
		level->playSound(pos, "random.bow", 1.0f, 1.2f);
	}
	else if (var12.getId() == Item::snowBall->m_itemID)
	{
		Snowball* var35 = new Snowball(level, exitPos, true);
		var35->shoot(var9, 0.1f, var10, 1.1f, 6.0f);
		level->addEntity(var35);
		level->playSound(pos, "random.bow", 1.0f, 1.2f);
	}
	else
	{
		ItemEntity* var36 = new ItemEntity(level, Vec3(exitPos.x, exitPos.y - 0.3f, exitPos.z), var12);
		var20 = random->nextFloat() * 0.1f + 0.2f;
		var36->m_vel.x = var9 * var20;
		var36->m_vel.y = 0.2f;
		var36->m_vel.z = var10 * var20;
		var36->m_vel.x += random->nextGaussian() * 0.0075f * 6.0f;
		var36->m_vel.y += random->nextGaussian() * 0.0075f * 6.0f;
		var36->m_vel.z += random->nextGaussian() * 0.0075f * 6.0f;
		level->addEntity(var36);
		level->playSound(pos, "random.click");
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

		level->addParticle("smoke", smokePos, smokeDir);
	}
}

void DispenserTile::neighborChanged(Level* level, const TilePos& pos, TileID tile)
{
	if (tile > 0 && Tile::tiles[tile]->isSignalSource())
	{
		bool var6 = level->hasNeighborSignal(pos) || level->hasNeighborSignal(pos.above());
		if (var6)
		{
			level->addToTickNextTick(pos, m_ID, getTickDelay());
		}
	}
}

void DispenserTile::tick(Level* level, const TilePos& pos, Random* random)
{
	if (level->hasNeighborSignal(pos) || level->hasNeighborSignal(pos.above()))
	{
		fireArrow(level, pos, random);
	}
}

TileEntity* DispenserTile::newTileEntity()
{
	return new DispenserTileEntity();
}

void DispenserTile::setPlacedBy(Level* level, const TilePos& pos, Mob* mob)
{
	int rot = Mth::floor(0.5f + (mob->m_rot.yaw * 4.0f / 360.0f)) & 3;

	TileData data = 0;

	switch (rot)
	{
	case 0: data = 2; break;
	case 1: data = 5; break;
	case 2: data = 3; break;
	case 3: data = 4; break;
	}

	level->setData(pos, data);
}

bool DispenserTile::hasTileEntity() const
{
    return true;
}
