/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "Material.hpp"
#include "world/item/Tool.hpp"

Material::Material(MapColor* mapColor) 
	: m_pMapColor(mapColor)
	, m_bFlammable(false)
	, m_bReplaceable(false)
	, m_bMineable(true)
	, m_bTranslucent(false)
	, m_pushReaction(PushReaction::NORMAL)
	, m_toolMask(Tool::NONE)
	, m_requiredToolLevel(0)
{}

Material::~Material()
{
}

Material
* Material::air,
* Material::dirt,
* Material::grass,
* Material::wood,
* Material::stone,
* Material::metal,
* Material::water,
* Material::lava,
* Material::leaves,
* Material::plant,
* Material::sponge,
* Material::cloth,
* Material::fire,
* Material::sand,
* Material::decoration,
* Material::glass,
* Material::explosive,
* Material::coral,
* Material::ice,
* Material::topSnow,
* Material::snow,
* Material::cactus,
* Material::clay,
* Material::vegetable,
* Material::portal,
* Material::cake,
* Material::web,
* Material::piston;

void Material::initMaterials()
{
	MapColor::initMapColors();

	air        = new GasMaterial(MapColor::air);

	grass       = (new Material(MapColor::grass))
		->setToolTypes(Tool::SHOVEL);

	dirt       = (new Material(MapColor::dirt))
		->setToolTypes(Tool::SHOVEL);

	wood       = (new Material(MapColor::wood))
		->setFlammable()
		->setToolTypes(Tool::HATCHET);

	stone      = (new Material(MapColor::stone))
		->setToolTypes(Tool::PICKAXE)
		->setNotAlwaysDestroyable();

	metal      = (new Material(MapColor::metal))
		->setNotAlwaysDestroyable();

	water = (new LiquidMaterial(MapColor::water))
		->destroyOnPush();

	lava = (new LiquidMaterial(MapColor::red))
		->destroyOnPush();

	leaves = (new Material(MapColor::foliage))
		->setFlammable()
		->setTranslucent()
		->destroyOnPush();

	plant = (new DecorationMaterial(MapColor::foliage))
		->destroyOnPush();

	sponge = new Material(MapColor::cloth);

	cloth = (new Material(MapColor::cloth))
		->setFlammable();

	fire = (new GasMaterial(MapColor::air))
		->destroyOnPush();

	sand       = (new Material(MapColor::sand))
		->setToolTypes(Tool::SHOVEL);

	decoration = (new DecorationMaterial(MapColor::air))
		->destroyOnPush();

	glass = (new Material(MapColor::air))
		->setTranslucent();

	explosive = (new Material(MapColor::red))
		->setFlammable()
		->setTranslucent();

	coral = (new Material(MapColor::foliage))
		->destroyOnPush();

	ice = (new Material(MapColor::ice))
		->setTranslucent();

	topSnow    = (new DecorationMaterial(MapColor::snow))
		->setToolTypes(Tool::SHOVEL)
		->setNotAlwaysDestroyable()
		->destroyOnPush();

	snow       = (new Material(MapColor::snow))
		->setToolTypes(Tool::SHOVEL)
		->setNotAlwaysDestroyable();

	cactus	   = (new Material(MapColor::foliage))
		->setTranslucent()
		->destroyOnPush();

	clay       = (new Material(MapColor::clay))
		->setToolTypes(Tool::SHOVEL);

	vegetable  = (new Material(MapColor::foliage))
		->destroyOnPush();

	portal     = new Material(MapColor::air);

	cake	   = (new Material(MapColor::cloth))
		->destroyOnPush();

	web        = (new Material(MapColor::stone))
		->setToolTypes(Tool::SWORD)
		->setNotAlwaysDestroyable();

	piston	   = (new Material(MapColor::stone))
		->notPushable();
}

void Material::teardownMaterials()
{
	if (air)        delete air;
	if (grass)      delete grass;
	if (dirt)       delete dirt;
	if (wood)       delete wood;
	if (stone)      delete stone;
	if (metal)      delete metal;
	if (water)      delete water;
	if (lava)       delete lava;
	if (leaves)     delete leaves;
	if (plant)      delete plant;
	if (sponge)     delete sponge;
	if (cloth)      delete cloth;
	if (fire)       delete fire;
	if (sand)       delete sand;
	if (decoration) delete decoration;
	if (glass)      delete glass;
	if (explosive)  delete explosive;
	if (coral)      delete coral;
	if (ice)        delete ice;
	if (topSnow)    delete topSnow;
	if (snow)       delete snow;
	if (cactus)     delete cactus;
	if (clay)       delete clay;
	if (vegetable)  delete vegetable;
	if (portal)     delete portal;
	if (cake)       delete cake;
	if (web)        delete web;
	if (piston)     delete piston;
}

bool Material::isLiquid() const
{
	return false;
}

bool Material::isMineable() const
{
	return m_bMineable;
}

bool Material::letsWaterThrough() const
{
	return !m_bTranslucent && isSolid();
}

bool Material::isSolid() const
{
	return true;
}

bool Material::isFlammable() const
{
	return m_bFlammable;
}

Material* Material::setNotAlwaysDestroyable()
{
	m_bMineable = false;
	return this;
}

Material* Material::setToolTypes(unsigned int toolMask)
{
	m_toolMask |= toolMask;
	return this;
}

Material* Material::setToolLevel(int toolLevel)
{
	m_requiredToolLevel = toolLevel;
	return this;
}

Material* Material::setToolTypesAndLevel(unsigned int toolMask, int toolLevel)
{
	setToolTypes(toolMask);
	setToolLevel(toolLevel);
	return this;
}

bool Material::blocksLight() const
{
	return true;
}

bool Material::blocksMotion() const
{
	return true;
}

Material* Material::setFlammable()
{
	m_bFlammable = true;
	return this;
}

Material* Material::setReplaceable()
{
	m_bReplaceable = true;
	return this;
}

Material* Material::setTranslucent()
{
	m_bTranslucent = true;
	return this;
}

Material* Material::destroyOnPush()
{
	m_pushReaction = PushReaction::DESTROY;
	return this;
}

Material* Material::notPushable()
{
	m_pushReaction = PushReaction::BLOCK;
	return this;
}

GasMaterial::GasMaterial(MapColor* mapColor) : Material(mapColor)
{
	setReplaceable();
}

bool GasMaterial::isSolid() const
{
	return false;
}

bool GasMaterial::blocksLight() const
{
	return false;
}

bool GasMaterial::blocksMotion() const
{
	return false;
}

DecorationMaterial::DecorationMaterial(MapColor* mapColor) : Material(mapColor)
{}

bool DecorationMaterial::isSolid() const
{
	return false;
}

bool DecorationMaterial::blocksLight() const
{
	return false;
}

bool DecorationMaterial::blocksMotion() const
{
	return false;
}

LiquidMaterial::LiquidMaterial(MapColor* mapColor) : Material(mapColor)
{
	setReplaceable();
}

bool LiquidMaterial::isLiquid() const
{
	return true;
}

bool LiquidMaterial::isSolid() const
{
	return false;
}

bool LiquidMaterial::blocksMotion() const
{
	return false;
}

