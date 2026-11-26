#pragma once

#include <vector>
#include "../Screen.hpp"
#include "client/gui/components/Button.hpp"

// Lets the player pick a texture pack from the resource_packs folder.
class TexturePackScreen : public Screen
{
public:
	TexturePackScreen(Screen* parent);
	~TexturePackScreen();

private:
	void _findPacks();
	void _clearPackButtons();
	void _updateButtonLabels();

public:
	void init() override;
	void render(float f) override;
	bool handleBackEvent(bool b) override;
	void _buttonClicked(Button* pButton) override;

private:
	Screen* m_pParent;
	std::vector<std::string> m_packs; // "" is the default pack
	std::vector<Button*> m_packButtons;
	std::string m_selected;
	std::string m_original;

public:
	Button m_btnDone;
};
