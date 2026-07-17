#include "PaintingType.hpp"

std::vector<PaintingType> PaintingType::types;

PaintingType::PaintingType(const std::string& name, int width, int height, int uo, int vo) 
    : m_name(name)
    , m_width(width)
    , m_height(height)
    , m_uo(uo)
    , m_vo(vo)
{
}

void PaintingType::registerType(const std::string& name, int width, int height, int uo, int vo)
{
    types.push_back(PaintingType(name, width, height, uo, vo));
}

void PaintingType::initTypes()
{
    registerType("Kebab", 16, 16, 0, 0);
    registerType("Aztec", 16, 16, 16, 0);
    registerType("Alban", 16, 16, 32, 0);
    registerType("Aztec2", 16, 16, 48, 0);
    registerType("Bomb", 16, 16, 64, 0);
    registerType("Plant", 16, 16, 80, 0);
    registerType("Wasteland", 16, 16, 96, 0);
    registerType("Pool", 32, 16, 0, 32);
    registerType("Courbet", 32, 16, 32, 32);
    registerType("Sea", 32, 16, 64, 32);
    registerType("Sunset", 32, 16, 96, 32);
    registerType("Creebet", 32, 16, 128, 32);
    registerType("Wanderer", 16, 32, 0, 64);
    registerType("Graham", 16, 32, 16, 64);
    registerType("Match", 32, 32, 0, 128);
    registerType("Bust", 32, 32, 32, 128);
    registerType("Stage", 32, 32, 64, 128);
    registerType("Void", 32, 32, 96, 128);
    registerType("SkullAndRoses", 32, 32, 128, 128);
    registerType("Fighters", 64, 32, 0, 96);
    registerType("Pointer", 64, 64, 0, 192);
    registerType("Pigscene", 64, 64, 64, 192);
    registerType("BurningSkull", 64, 64, 128, 192);
    registerType("Skeleton", 64, 48, 192, 64);
    registerType("DonkeyKong", 64, 48, 192, 112);
}
