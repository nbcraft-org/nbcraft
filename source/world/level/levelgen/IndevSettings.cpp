/********************************************************************
	NBCraft

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include "IndevSettings.hpp"

bool Indev::IsTileAvailable(int tileId)
{
	switch (tileId)
	{
	case TILE_STONE:
	case TILE_GRASS:
	case TILE_DIRT:
	case TILE_STONEBRICK: // cobblestone
	case TILE_WOOD:
	case TILE_SAPLING:
	case TILE_BEDROCK:
	case TILE_WATER:
	case TILE_WATER_CALM:
	case TILE_LAVA:
	case TILE_LAVA_CALM:
	case TILE_SAND:
	case TILE_GRAVEL:
	case TILE_ORE_GOLD:
	case TILE_ORE_IRON:
	case TILE_ORE_COAL:
	case TILE_TREE_TRUNK:
	case TILE_LEAVES:
	case TILE_SPONGE:
	case TILE_GLASS:
	case TILE_CLOTH:
	case TILE_FLOWER:
	case TILE_ROSE:
	case TILE_MUSHROOM_1:
	case TILE_MUSHROOM_2:
	case TILE_BLOCK_GOLD:
	case TILE_BLOCK_IRON:
	case TILE_STONESLAB_FULL:
	case TILE_STONESLAB_HALF:
	case TILE_BRICKS:
	case TILE_TNT:
	case TILE_BOOKSHELF:
	case TILE_MOSS_STONE:
	case TILE_OBSIDIAN:
	case TILE_TORCH:
	case TILE_FIRE:
	case TILE_SPAWNER:
	case TILE_CHEST:
	case TILE_ORE_EMERALD: // diamond ore
	case TILE_BLOCK_EMERALD: // diamond block
	case TILE_WORKBENCH:
	case TILE_WHEAT:
	case TILE_FARMLAND:
	case TILE_FURNACE:
	case TILE_FURNACE_LIT:
	case TILE_DOOR_WOOD:
	case TILE_INVISIBLE:
	case TILE_LEAVES_CARRIED:
		return true;
	default:
		return false;
	}
}

bool Indev::IsItemAvailable(int itemId)
{
	if (itemId < 256)
		return IsTileAvailable(itemId);

	switch (itemId)
	{
	case ITEM_SHOVEL_IRON:
	case ITEM_PICKAXE_IRON:
	case ITEM_HATCHET_IRON:
	case ITEM_FLINT_AND_STEEL:
	case ITEM_APPLE:
	case ITEM_BOW:
	case ITEM_ARROW:
	case ITEM_COAL:
	case ITEM_EMERALD: // diamond
	case ITEM_INGOT_IRON:
	case ITEM_INGOT_GOLD:
	case ITEM_SWORD_IRON:
	case ITEM_SWORD_WOOD:
	case ITEM_SHOVEL_WOOD:
	case ITEM_PICKAXE_WOOD:
	case ITEM_HATCHET_WOOD:
	case ITEM_SWORD_STONE:
	case ITEM_SHOVEL_STONE:
	case ITEM_PICKAXE_STONE:
	case ITEM_HATCHET_STONE:
	case ITEM_SWORD_EMERALD:
	case ITEM_SHOVEL_EMERALD:
	case ITEM_PICKAXE_EMERALD:
	case ITEM_HATCHET_EMERALD:
	case ITEM_STICK:
	case ITEM_BOWL:
	case ITEM_STEW_MUSHROOM:
	case ITEM_SWORD_GOLD:
	case ITEM_SHOVEL_GOLD:
	case ITEM_PICKAXE_GOLD:
	case ITEM_HATCHET_GOLD:
	case ITEM_STRING:
	case ITEM_FEATHER:
	case ITEM_SULPHUR:
	case ITEM_HOE_WOOD:
	case ITEM_HOE_STONE:
	case ITEM_HOE_IRON:
	case ITEM_HOE_EMERALD:
	case ITEM_HOE_GOLD:
	case ITEM_SEEDS:
	case ITEM_WHEAT:
	case ITEM_BREAD:
	case ITEM_HELMET_CLOTH:
	case ITEM_CHESTPLATE_CLOTH:
	case ITEM_LEGGINGS_CLOTH:
	case ITEM_BOOTS_CLOTH:
	case ITEM_HELMET_CHAIN:
	case ITEM_CHESTPLATE_CHAIN:
	case ITEM_LEGGINGS_CHAIN:
	case ITEM_BOOTS_CHAIN:
	case ITEM_HELMET_IRON:
	case ITEM_CHESTPLATE_IRON:
	case ITEM_LEGGINGS_IRON:
	case ITEM_BOOTS_IRON:
	case ITEM_HELMET_EMERALD:
	case ITEM_CHESTPLATE_EMERALD:
	case ITEM_LEGGINGS_EMERALD:
	case ITEM_BOOTS_EMERALD:
	case ITEM_HELMET_GOLD:
	case ITEM_CHESTPLATE_GOLD:
	case ITEM_LEGGINGS_GOLD:
	case ITEM_BOOTS_GOLD:
	case ITEM_FLINT:
	case ITEM_PORKCHOP_RAW:
	case ITEM_PORKCHOP_COOKED:
	case ITEM_PAINTING:
	case ITEM_APPLE_GOLD:
	case ITEM_DOOR_WOOD:
	case ITEM_SPAWN_EGG:
		return true;
	default:
		return false;
	}
}
