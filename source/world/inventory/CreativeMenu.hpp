#pragma once

#include "CreativeMenuBase.hpp"

class CreativeMenu : public CreativeMenuBase
{
public:
    CreativeMenu(Container* inventory, Container* container);

public:
    void updateScroll(float scroll);

private:
    Container* m_container;
};
