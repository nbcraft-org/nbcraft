/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "Tile.hpp"

class RocketLauncherTile : public Tile
{
public:
	RocketLauncherTile(TileID id);

private:
	bool _use(TileSource& source, const TilePos& pos);

public:
	int getTexture(Facing::Name face, TileData data) const override;
	AABB* getAABB(TileSource*, const TilePos& pos) override;
	eRenderShape getRenderShape() const override;
	bool isCubeShaped() const override;
	bool isSolidRender() const override;
	bool isSignalSource() const override;
	bool use(const TilePos& pos, Player* player) override;
	void neighborChanged(TileSource* source, const TilePos& pos, TileID newTile) override;
	void tick(TileSource*, const TilePos& pos, Random*) override;
	int getTickDelay() const override;
};
