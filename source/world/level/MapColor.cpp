#include "MapColor.hpp"
#include "common/Utils.hpp"

MapColor
* MapColor::air,
* MapColor::grass,
* MapColor::sand,
* MapColor::cloth,
* MapColor::red,
* MapColor::ice,
* MapColor::metal,
* MapColor::foliage,
* MapColor::snow,
* MapColor::clay,
* MapColor::dirt,
* MapColor::stone,
* MapColor::water,
* MapColor::wood;

MapColor* MapColor::mapColors[16];

MapColor::MapColor(int id, int color) 
	: m_id(id)
	, m_color(color)
{
	mapColors[id] = this;
}

void MapColor::initMapColors()
{
	air = new MapColor(0, 0);
	grass = new MapColor(1, 0x7FB238);
	sand = new MapColor(2, 0xF7E9A3);
	cloth = new MapColor(3, 0xA7A7A7);
	red = new MapColor(4, 0xFF0000);
	ice = new MapColor(5, 0xA0A0FF);
	metal = new MapColor(6, 0xA7A7A7);
	foliage = new MapColor(7, 0x007C00);
	snow = new MapColor(8, 0xFFFFFF);
	clay = new MapColor(9, 0xA4A8B8);
	dirt = new MapColor(10, 0xB76A2F);
	stone = new MapColor(11, 0x707070);
	water = new MapColor(12, 0x4040FF);
	wood = new MapColor(13, 0x685332);
}

void MapColor::teardownMapColors()
{
	for (size_t i = 0; i < 16; ++i)
		SAFE_DELETE(mapColors[i]);
}
