#pragma once

#include "PanelScreen_Console.hpp"
#include "client/gui/components/Button.hpp"
#include "client/gui/components/TickBox.hpp"
#include "client/gui/components/SliderButton.hpp"
#include "client/gui/components/TextBox.hpp"

class LoadSaveScreen_Console : public PanelScreen_Console
{
public:
	LoadSaveScreen_Console(Options& options, Screen* parent, const LevelSummary& summary);

protected:
	void _buttonClicked(Button* pButton) override;

public:
	void init() override;
	void render(float f) override;
	void renderPanel(float f) override;

public:
	TickBox m_onlineGame;
	TickBox m_inviteOnly;
	SliderButton m_difficultySlider;
	Button m_btnLoad;
	LevelSummary m_summary;
};
