#include "TrapdoorTile.hpp"
#include "world/level/Level.hpp"
#include "world/item/Item.hpp"

TrapdoorTile::TrapdoorTile(int ID, Material* pMtl) : Tile(ID, pMtl)
{
	m_TextureFrame = TEXTURE_TRAPDOOR;
	m_renderLayer = RENDER_LAYER_ALPHATEST;

	if (pMtl == Material::metal)
		m_TextureFrame = TEXTURE_TRAPDOOR + 1; // in beta too for some reason even though iron trapdoors don't exist

	Tile::setShape(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

AABB* TrapdoorTile::getAABB(const Level* level, const TilePos& pos)
{
	updateShape(level, pos);
	return Tile::getAABB(level, pos);
}

AABB TrapdoorTile::getTileAABB(const Level* level, const TilePos& pos)
{
	updateShape(level, pos);
	return Tile::getTileAABB(level, pos);
}

void TrapdoorTile::updateShape(const LevelSource* level, const TilePos& pos)
{
	TileData metadata = level->getData(pos);
	float f = 0.1875F;
	setShape(0.0F, 0.0F, 0.0F, 1.0F, f, 1.0F);
	if (isOpen(metadata)) {
		if ((metadata & 3) == 0) {
			setShape(0.0F, 0.0F, 1.0F - f, 1.0F, 1.0F, 1.0F);
		}

		if ((metadata & 3) == 1) {
			setShape(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, f);
		}

		if ((metadata & 3) == 2) {
			setShape(1.0F - f, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
		}

		if ((metadata & 3) == 3) {
			setShape(0.0F, 0.0F, 0.0F, f, 1.0F, 1.0F);
		}
	}
}

void TrapdoorTile::updateDefaultShape()
{
	float f = 0.1875F;
	Tile::setShape(0.0f, 0.5f - f / 2.0f, 0.0f, 1.0f, 0.5f + f / 2.0f, 1.0f);
}

void TrapdoorTile::attack(Level* level, const TilePos& pos, Player* player)
{
	use(level, pos, player);
}

void TrapdoorTile::setOpen(Level* level, const TilePos& pos, bool bOpen)
{
	TileData data = level->getData(pos);
	int j = (data & 4) > 0 ? 1 : 0;
	if (isOpen(level->getData(pos)) != bOpen)
	{
		level->setData(pos, data ^ 4);
		level->levelEvent(LevelEvent(LevelEvent::SOUND_DOOR, pos, 0));
	}
}

void TrapdoorTile::neighborChanged(Level* level, const TilePos& pos, TileID newTile) // check this
{
	int i = level->getData(pos);
	TilePos newPos = pos;
	if ((i & 3) == 0) {
		newPos.z = pos.z + 1;
	}

	if ((i & 3) == 1) {
		newPos.z--;
	}

	if ((i & 3) == 2) {
		newPos.x = pos.x + 1;
	}

	if ((i & 3) == 3) {
		newPos.x--;
	}

	if (!level->isSolidTile(newPos)) {
		level->setTile(pos, 0);
		spawnResources(level, pos, level->getData(pos));
	}

	if (newTile > 0 && Tile::tiles[newTile]->isSignalSource()) {
		int l = level->hasNeighborSignal(pos);
		setOpen(level, pos, (bool)l);
	}
}

HitResult TrapdoorTile::clip(const Level* level, const TilePos& pos, Vec3 v1, Vec3 v2)
{
	updateShape(level, pos);
	return Tile::clip(level, pos, v1, v2);
}

void TrapdoorTile::setPlacedOnFace(Level* level, const TilePos& pos, Facing::Name face) // check this
{
	int i = 0;
	if (face == 2) {
		i = 0;
	}

	if (face == 3) {
		i = 1;
	}

	if (face == 4) {
		i = 2;
	}

	if (face == 5) {
		i = 3;
	}

	level->setData(pos, i);
}

bool TrapdoorTile::mayPlace(const Level* level, const TilePos& pos) const // check this
{
	if (level->isSolidTile(pos.west())) return true;
	if (level->isSolidTile(pos.east())) return true;
	if (level->isSolidTile(pos.north())) return true;
	if (level->isSolidTile(pos.south())) return true;

	return false;
}

bool TrapdoorTile::use(Level* level, const TilePos& pos, Player* player)
{
	// again somewhat of a useless check since iron trapdoors don't exist
	if (m_pMaterial == Material::metal)
		return true;

	TileData data = level->getData(pos);
	level->setData(pos, data ^ 4);
	level->levelEvent(LevelEvent(LevelEvent::SOUND_DOOR, pos, 0, player));
	return true;
}