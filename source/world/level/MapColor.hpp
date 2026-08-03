#pragma once

struct MapColor
{
	MapColor(int id, int color);

	static void initMapColors();
	static void teardownMapColors();

	static MapColor* mapColors[16];
	static MapColor
		*air,
		*grass,
		*sand,
		*cloth,
		*red,
		*ice,
		*metal,
		*foliage,
		*snow,
		*clay,
		*dirt,
		*stone,
		*water,
		*wood;

	const int id;
	const int color;
};
