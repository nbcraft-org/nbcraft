#pragma once

#include "../Screen.hpp"
#include "client/gui/components/Button.hpp"

class DeathScreen_Console : public Screen
{
public:
	DeathScreen_Console();

protected:
	void _buttonClicked(Button* pButton) override;

public:
	void init() override;
	void tick() override;
	void render(float f) override;

private:
	int m_tickCounter;
	std::string m_header;
	Button m_btnRespawn;
	Button m_btnTitle;
};
