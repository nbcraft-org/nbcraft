#pragma once

#include "../Screen.hpp"

class ProgressScreen_Console : public Screen
{
public:
	void render(float f) override;
	void updateEvents() override;
	bool isInGameScreen() override;
	void init() override;
public:
	virtual std::string updateText();
private:
	std::string m_loadingTip;
	std::vector<std::string> m_loadingTipLines;
public:
	std::string m_header;
};

