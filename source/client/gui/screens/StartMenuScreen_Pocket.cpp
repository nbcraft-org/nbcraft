#include "StartMenuScreen_Pocket.hpp"

#include "client/gui/components/LargeImageButton.hpp"

#define C_PRIMARY_TOUCH_BTN_SIZE 75

StartMenuScreen_Pocket::StartMenuScreen_Pocket()
{
	m_uiTheme = UI_POCKET;
}

StartMenuScreen_Pocket::~StartMenuScreen_Pocket()
{
}

void StartMenuScreen_Pocket::_setup()
{
	LargeImageButton *startBtn, *joinBtn, *optionsBtn;
	m_startButton   = startBtn   = new LargeImageButton("Start Game");
	m_joinButton    = joinBtn    = new LargeImageButton("Join Game");
	m_optionsButton = optionsBtn = new LargeImageButton("Options");

	ImageDef image;
	{
		image.name = "gui/touchgui.png";
		image.width = C_PRIMARY_TOUCH_BTN_SIZE;
		image.height = C_PRIMARY_TOUCH_BTN_SIZE;
		image.bUseCustomUV = true;
		image.uv = IntRectangle(0, 26, C_PRIMARY_TOUCH_BTN_SIZE, C_PRIMARY_TOUCH_BTN_SIZE);
	}

	optionsBtn->setImageDef(image, true);

	image.uv.y += image.uv.h;
	startBtn->setImageDef(image, true);

	image.uv.y += image.uv.h;
	joinBtn->setImageDef(image, true);

	_setupPositions();
}

void StartMenuScreen_Pocket::_setupPositions()
{
	int startBtnWidth = m_startButton->m_width;

	int v5 = m_height / 3 + 2;
	m_startButton->m_yPos   = v5;
	m_joinButton->m_yPos    = v5;
	m_optionsButton->m_yPos = v5;
	m_buyButton.m_yPos      = m_height - (m_buyButton.m_height + 4);

	float v6 = ((float)m_width + ((float)startBtnWidth * -3.0f)) * 0.25f;
	m_joinButton->m_xPos    = v6;
	m_startButton->m_xPos   = (v6 * 2.0f) + startBtnWidth;
	m_optionsButton->m_xPos = (v6 * 3.0f) + (startBtnWidth * 2);
	m_buyButton.m_xPos      = (v6 * 2.0f) + startBtnWidth;
}
