/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "EntityRenderer.hpp"
#include "../TileRenderer.hpp"
#include "world/item/ItemStack.hpp"

class ItemRenderer : public EntityRenderer
{
protected:
	class Materials
	{
	public:
		mce::MaterialPtr item_entity_item;
		mce::MaterialPtr item_entity_item_layered;
		mce::MaterialPtr item_entity_tile;
		mce::MaterialPtr ui_fill_color;
		mce::MaterialPtr ui_fill_gradient;
		mce::MaterialPtr ui_textured;
		mce::MaterialPtr ui_texture_and_color;
		mce::MaterialPtr ui_item; // only supposed to be in TileRenderer
		mce::MaterialPtr ui_item_glint;

		Materials();
	};

private:
	static ItemRenderer* singletonPtr;
public:
	static ItemRenderer& singleton();

public:
	ItemRenderer();

	void render(const Entity& entity, const Vec3& pos, float rot, float a) override;

	void blitRect(Tesselator&, int, int, int, int, int);
	void blit(int dx, int dy, int sx, int sy, int tw, int th, const Color&);

	void renderGuiItem(Minecraft&, const ItemStack&, int x, int y, const Color& = Color::WHITE);
	void renderGuiItemText(Minecraft&, const ItemStack&, int x, int y, bool isLimited = true, bool drawShadow = true);
	void renderGuiItemOverlay(Minecraft&, const ItemStack&, int x, int y);

private:
	TileRenderer* m_pTileRenderer;
	Materials m_itemMaterials;
public:
	Random m_random;
};

