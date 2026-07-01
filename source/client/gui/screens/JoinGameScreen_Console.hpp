#pragma once

#include "PanelScreen_Console.hpp"
#include "client/gui/components/Button.hpp"
#include "client/gui/components/TickBox.hpp"
#include "client/gui/components/SliderButton.hpp"
#include "client/gui/components/TextBox.hpp"
class UsernameButton : public Button
{
public:
	UsernameButton(const std::string&);

	void render(Minecraft*, const MenuPointer&) override;
	virtual void renderMessage(Font&, const Color&);
};

class JoinGameScreen_Console : public PanelScreen_Console
{
public:
	JoinGameScreen_Console(Screen*, const PingedCompatibleServer& server);

protected:
	void _buttonClicked(Button* pButton) override;

public:
	void init() override;
	void render(float f) override;
	void renderPanel(float f) override;

public:
	Button m_btnLoad;
	Button m_btnJoinGame;
	UsernameButton m_btnUsername;

private:
	PingedCompatibleServer m_server;
	float m_panelAlpha = 1.0f;
};
