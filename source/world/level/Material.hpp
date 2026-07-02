/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "compat/LegacyCPP.hpp"
#include "MapColor.hpp"

enum class PushReaction
{
	NORMAL,
	DESTROY,
	BLOCK
};

class Material
{
public:
	Material(MapColor* mapColor);
	virtual ~Material();

	static void initMaterials();
	static void teardownMaterials();

	virtual bool isLiquid() const;
	virtual bool isMineable() const;
	virtual bool letsWaterThrough() const;
	virtual bool isSolid() const;
	virtual bool blocksLight() const;
	virtual bool blocksMotion() const;
	virtual bool isFlammable() const;
	virtual Material* setReplaceable();
	virtual Material* setFlammable();
	virtual Material* setNotAlwaysDestroyable();
	virtual Material* setTranslucent();
	virtual Material* destroyOnPush();
	virtual Material* notPushable();

	Material* setToolTypes(unsigned int toolMask);
	Material* setToolLevel(int toolLevel);
	Material* setToolTypesAndLevel(unsigned int toolMask, int toolLevel = 0);

public:
	static Material
		*air,
		*grass,
		*dirt,
		*wood,
		*stone,
		*metal,
		*water,
		*lava,
		*leaves,
		*plant,
		*sponge,
		*cloth,
		*fire,
		*sand,
		*decoration,
		*glass,
		*explosive,
		*coral,
		*ice,
		*topSnow,
		*snow,
		*cactus,
		*clay,
		*vegetable,
		*portal,
		*cake,
		*web,
		*piston;

public:
	const MapColor* m_pMapColor;
	bool m_bFlammable;
	bool m_bReplaceable;
	bool m_bMineable;
	bool m_bTranslucent;
	PushReaction m_pushReaction;
	unsigned int m_toolMask;
	int m_requiredToolLevel;
};

class GasMaterial : public Material
{
public:
	GasMaterial(MapColor*);
	bool isSolid() const override;
	bool blocksLight() const override;
	bool blocksMotion() const override;
};

class LiquidMaterial : public Material
{
public:
	LiquidMaterial(MapColor*);
	bool isLiquid() const override;
	bool isSolid() const override;
	bool blocksMotion() const override;
};

class DecorationMaterial : public Material
{
public:
	DecorationMaterial(MapColor*);
	bool isSolid() const override;
	bool blocksLight() const override;
	bool blocksMotion() const override;
};
