#pragma once
#include "client/renderer/Font.hpp"

class ConsoleFont : public Font
{
public:
	ConsoleFont(Options* options, const std::string& fileName, Textures* textures);
	virtual ~ConsoleFont();

	void drawScalableShadow(const std::string& str, int x, int y, const Color& color, float scale) override;
	void drawString(const std::string& str, int x, int y, const Color& color, bool hasShadow) override;
};
