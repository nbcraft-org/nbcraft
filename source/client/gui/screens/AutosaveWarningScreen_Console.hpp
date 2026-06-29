#pragma once

#include "PanelScreen_Console.hpp"
#include "client/gui/components/Button.hpp"
#include "client/gui/components/TickBox.hpp"
#include "client/gui/components/SliderButton.hpp"
#include "client/gui/components/TextBox.hpp"

class AutosaveWarningScreen_Console : public PanelScreen_Console
{
public:
	AutosaveWarningScreen_Console(Screen*);

protected:
	void _buttonClicked(Button* pButton) override;

public:
	void init() override;
	void render(float f) override;
	void renderPanel(float f) override;

public:
	Button m_btnLoad;
};
