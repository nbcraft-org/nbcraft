#pragma once

#include <vector>
#include <string>

struct PaintingType
{
	PaintingType(const std::string& name, int width, int height, int uo, int vo)
		: m_name(name)
		, m_width(width)
		, m_height(height)
		, m_uo(uo)
		, m_vo(vo)
	{
	}

	std::string m_name;
	int m_width, m_height, m_uo, m_vo;

	static void initTypes();

	static void registerType(const std::string& name, int width, int height, int uo, int vo)
	{
		types.push_back(PaintingType(name, width, height, uo, vo));
	}

	static std::vector<PaintingType> types;
};
