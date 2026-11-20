#include "SpawnEggItem.hpp"
#include "common/Util.hpp"
#include "client/locale/Language.hpp"
#include "world/level/Level.hpp"
#include "world/tile/Tile.hpp"
#include "world/entity/Player.hpp"
#include "world/entity/MobFactory.hpp"
#include "world/entity/EntityTypeDescriptor.hpp"

std::map<EntityType::ID, SpawnEggItem::Type> SpawnEggItem::eggTypes = std::map<EntityType::ID, SpawnEggItem::Type>();

SpawnEggItem::Type::Type(EntityType::ID spawnedType, const Color& primaryColor, const Color& secondaryColor)
	: m_spawnedType(spawnedType)
	, m_primaryColor(primaryColor)
	, m_secondaryColor(secondaryColor)
{
}

void SpawnEggItem::_AddEgg(EntityType::ID spawnedType, const Color& primaryColor, const Color& secondaryColor)
{
	eggTypes.insert(std::make_pair(spawnedType, Type(spawnedType, primaryColor, secondaryColor)));
}

const SpawnEggItem::Type* SpawnEggItem::GetTypeByEntityTypeID(EntityType::ID id)
{
	std::map<EntityType::ID, Type>::const_iterator it = eggTypes.find(id);
	if (it != eggTypes.end())
		return &it->second;

	return nullptr;
}

void SpawnEggItem::initTypes()
{
	_AddEgg(EntityType::CREEPER,    Color::FromRGB( 13, 167,  11), Color::FromRGB(  0,   0,   0));
	_AddEgg(EntityType::SKELETON,   Color::FromRGB(193, 193, 193), Color::FromRGB( 73,  73,  73));
	_AddEgg(EntityType::SPIDER,     Color::FromRGB( 52,  45,  38), Color::FromRGB(168,  14,  14));
	_AddEgg(EntityType::ZOMBIE,     Color::FromRGB(  0, 175, 175), Color::FromRGB(121, 156, 101));
	// Indev mobs only
	_AddEgg(EntityType::PIG,        Color::FromRGB(240, 165, 162), Color::FromRGB(219,  99,  95));
	_AddEgg(EntityType::SHEEP,      Color::FromRGB(231, 231, 231), Color::FromRGB(255, 181, 181));
}

SpawnEggItem::SpawnEggItem(int itemID) : Item(itemID)
{
	m_bStackedByData = true;
	m_maxDamage = 0;
}

std::string SpawnEggItem::getHovertextName(ItemStack& item) const
{
	std::string entityName = "entity.unknown.name";
	EntityType::ID entityTypeId = (EntityType::ID)item.getAuxValue();
	const EntityTypeDescriptor* pTypeDesc = EntityTypeDescriptor::GetByEntityTypeID(entityTypeId);
	if (pTypeDesc)
		entityName = "entity." + pTypeDesc->getEntityType().getName() + ".name";

	return Util::format(Language::get(Item::getName()).c_str(), Language::get(entityName).c_str());
}

Color SpawnEggItem::getColor(const ItemStack* itemStack, int layer) const
{
	if (!itemStack)
		return Color::WHITE;

	const Type* pType = GetTypeByEntityTypeID((EntityType::ID)itemStack->getAuxValue());
	if (!pType)
		return Color::WHITE;

	switch (layer)
	{
	case 0: return pType->m_primaryColor;
	case 1: return pType->m_secondaryColor;
	}

	return Color::WHITE;
}

bool SpawnEggItem::useOn(ItemStack& itemStack, Player& player, const TilePos& pos, Facing::Name face) const
{
	Level& level = player.getLevel();
	TileSource& source = player.getTileSource();

	TilePos tp = pos.relative(face);
	Vec3 spawnPos(tp.x + 0.5f, float(tp.y), tp.z + 0.5f);

	Tile* pTile = Tile::tiles[source.getTile(pos)];
	if (face == Facing::UP && pTile)
	{
		const AABB* pAABB = pTile->getAABB(source, pos);
		if (pAABB)
			spawnPos.y = pAABB->max.y;
	}

	if (!_SpawnCreature(level, (EntityType::ID)itemStack.getAuxValue(), spawnPos))
		return false;

	if (!player.isCreative())
		itemStack.shrink();

	return true;
}

bool SpawnEggItem::_SpawnCreature(Level& level, EntityType::ID entityType, const Vec3& pos)
{
	if (level.m_bIsClientSide)
		return true;

	Mob* pMob = MobFactory::CreateMob(entityType, level);
	if (!pMob)
		return false;

	pMob->moveTo(pos, Rot2(level.m_random.nextFloat() * 360.0f, 0.0f));
	level.addEntity(pMob);

	pMob->playAmbientSound();

	return true;
}
