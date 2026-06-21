/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "Tile.hpp"

class ClothTile : public Tile
{
public:
	enum Color
	{
		WHITE, ORANGE, MAGENTA, LIGHT_BLUE, YELLOW, LIME, PINK, GRAY, SILVER, CYAN, PURPLE, BLUE, BROWN, GREEN, RED, BLACK
	};

	ClothTile(int id);

public:
	int getTexture(Facing::Name face, TileData data) const override;
	int getSpawnResourcesAuxValue(int val) const override;

public:
	static int getColorFromData(int var0)
	{
		return ~var0 & 15;
	}
};
