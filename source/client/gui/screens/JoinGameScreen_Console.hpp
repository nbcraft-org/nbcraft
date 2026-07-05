#pragma once

#include "PanelScreen_Console.hpp"
#include "client/gui/components/Button.hpp"

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

public:
	void init() override;
	void render(float f) override;
	void renderPanel(float f) override;

protected:
	void _buttonClicked(Button* pButton) override;

private:
	PingedCompatibleServer m_server;
	float m_panelAlpha;

public:
	Button m_btnJoinGame;
	UsernameButton m_btnUsername;
};
