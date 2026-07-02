#pragma once

class MapColor
{
public:
	MapColor(int, int);

	static void initMapColors();
	static void teardownMapColors();

public:
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

	const int m_id;
	const int m_color;
};
