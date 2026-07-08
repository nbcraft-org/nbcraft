#pragma once

#include "PanelScreen_Console.hpp"
#include "client/gui/components/Button.hpp"

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
	void tick() override;

private:
	std::string m_autosaveText;
	std::vector<std::string> m_autosaveLine;
	int m_timer;

public:
	Button m_btnLoad;
};
