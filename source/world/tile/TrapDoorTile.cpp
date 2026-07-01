#include "TrapDoorTile.hpp"
#include "world/item/Item.hpp"
#include "world/level/Level.hpp"
#include "world/level/TileSource.hpp"

TrapdoorTile::TrapdoorTile(int ID, Material* pMtl) : Tile(ID, pMtl)
{
	m_TextureFrame = TEXTURE_TRAPDOOR;
	m_renderLayer = RENDER_LAYER_ALPHATEST;

	if (pMtl == Material::metal)
		m_TextureFrame = TEXTURE_TRAPDOOR + 1; // Very likely a leftover from DoorTile.

	Tile::setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

AABB* TrapdoorTile::getAABB(TileSource& source, const TilePos& pos)
{
	updateShape(source, pos);
	return Tile::getAABB(source, pos);
}

AABB TrapdoorTile::getTileAABB(TileSource& source, const TilePos& pos)
{
	updateShape(source, pos);
	return Tile::getTileAABB(source, pos);
}

void TrapdoorTile::updateShape(TileSource& source, const TilePos& pos)
{
	TileData metadata = source.getExtraData(pos);
	constexpr float f = 0.1875f;
	setShape(0.0f, 0.0f, 0.0f, 1.0f, f, 1.0f);
	if (isOpen(metadata)) {
		switch (metadata & C_DIR_MASK)
		{
			case 0: setShape(0.0f, 0.0f, 1.0f - f, 1.0f, 1.0f, 1.0f); break;
			case 1: setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, f);		  break;
			case 2: setShape(1.0f - f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f); break;
			case 3: setShape(0.0f, 0.0f, 0.0f, f, 1.0f, 1.0f);		  break;
		}
	}
}

void TrapdoorTile::updateDefaultShape()
{
	constexpr float f = 0.1875f;
	Tile::setShape(0.0f, 0.5f - f / 2.0f, 0.0f, 1.0f, 0.5f + f / 2.0f, 1.0f);
}

void TrapdoorTile::attack(const TilePos& pos, Player* player)
{
	use(pos, player);
}

void TrapdoorTile::setOpen(TileSource& source, const TilePos& pos, bool bOpen)
{
	TileData data = source.getExtraData(pos);
	if (isOpen(data) != bOpen)
	{
		source.setTileAndData(pos, FullTile(this, data ^ C_OPEN_BIT));
		level->levelEvent(LevelEvent(LevelEvent::SOUND_DOOR, pos, 0));
	}
}

void TrapdoorTile::neighborChanged(TileSource& source, const TilePos& pos, TileID newTile) // check this
{
	int data = source.getExtraData(pos);
	TilePos newPos = pos;

	switch (data & C_DIR_MASK)
	{
		case 0: newPos.z = pos.z + 1; break;
		case 1: newPos.z--;           break;
		case 2: newPos.x = pos.x + 1; break;
		case 3: newPos.x--;           break;
		default: break;
	}

	if (!source.isSolidBlockingTile(newPos)) {
		source.setTile(pos, 0);
		spawnResources(source, pos, source.getData(pos));
	}

	if (newTile > 0 && Tile::tiles[newTile]->isSignalSource()) {
		int l = source.hasNeighborSignal(pos);
		setOpen(*source, pos, (bool)l);
	}
}

HitResult TrapdoorTile::clip(const Level* level, const TilePos& pos, Vec3 v1, Vec3 v2)
{
	updateShape(level, pos);
	return Tile::clip(level, pos, v1, v2);
}

void TrapdoorTile::setPlacedOnFace(Level* level, const TilePos& pos, Facing::Name face) // check this
{
	int updateFlag = 0;
	switch (face)
	{
		case (Facing::NORTH): updateFlag = 0; break;
		case (Facing::SOUTH): updateFlag = 1; break;
		case (Facing::WEST):  updateFlag = 2; break;
		case (Facing::EAST):  updateFlag = 3; break;
		default: break;
	}
	level->setData(pos, updateFlag);
}

bool TrapdoorTile::mayPlace(TileSource& source, const TilePos& pos) const // check this
{
	if (level->isSolidBlockingTile(pos.west())) return true;
	if (level->isSolidBlockingTile(pos.east())) return true;
	if (level->isSolidBlockingTile(pos.north())) return true;
	if (level->isSolidBlockingTile(pos.south())) return true;

	return false;
}

bool TrapdoorTile::use(TileSource& source, const TilePos& pos, Player* player)
{
	// Door Tile Leftover
	if (m_pMaterial == Material::metal)
		return true;

	TileData data = level->getData(pos);
	level->setData(pos, data ^ C_OPEN_BIT);
	level->levelEvent(LevelEvent(LevelEvent::SOUND_DOOR, pos, 0, player));
	return true;
}
