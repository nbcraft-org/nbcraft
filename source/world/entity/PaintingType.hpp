#pragma once

#include <vector>
#include <string>

struct PaintingType
{
	PaintingType(const std::string& name, int width, int height, int uo, int vo);

	std::string m_name;
	int m_width, m_height, m_uo, m_vo;

	static void initTypes();

	static void registerType(const std::string& name, int width, int height, int uo, int vo);

	static std::vector<PaintingType> types;
};