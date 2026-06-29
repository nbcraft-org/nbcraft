#pragma once

#include "../AreaNavigation.hpp"
#include "../GuiComponent.hpp"
#include "common/Mth.hpp"

class ScrollRenderer_Console : public GuiComponent
{
public:
    ScrollRenderer_Console();

public:
    void updateScroll(AreaNavigation::Direction);
    void renderScroll(AreaNavigation::Direction, Textures&, int x, int y);
    void renderScroll(AreaNavigation::Direction, Textures&, int x, int y, const std::string&, int width, int height);

public:
    int m_lastScrolled[4];

    static std::string SCROLLS[];
};