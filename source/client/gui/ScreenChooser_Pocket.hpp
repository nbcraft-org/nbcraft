#pragma once

#include "ScreenChooser.hpp"

class ScreenChooser_Pocket : public ScreenChooser
{
public:
    ScreenChooser_Pocket(Minecraft*);
    ~ScreenChooser_Pocket();

public:
    void pushStartScreen() override;
};