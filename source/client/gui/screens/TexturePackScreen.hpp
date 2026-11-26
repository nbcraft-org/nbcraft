#pragma once

#include <vector>
#include "../Screen.hpp"
#include "client/gui/components/Button.hpp"
#include "client/gui/components/ScrolledSelectionList.hpp"
#include "client/resources/ResourceLocation.hpp"

class TexturePackScreen;

struct TexturePackEntry
{
	std::string name;     // folder name, "" for the default pack
	std::string title;
	std::string desc1;
	std::string desc2;
	ResourceLocation iconLocation; // where its pack.png is
	std::string iconTexture;       // name the icon is uploaded under, "" if it has none
	bool iconMissing;

	TexturePackEntry() : iconMissing(false) {}
};

class TexturePackList : public ScrolledSelectionList
{
public:
	TexturePackList(Minecraft* pMinecraft, TexturePackScreen* pScreen, int width, int height, int y0, int y1, int itemHeight);

	int getNumberOfItems() override;
	bool isSelectedItem(int i) override;
	void renderBackground(float f) override;
	void renderItem(int index, int x, int y, int height, const MenuPointer& pointer, Tesselator& t) override;
	void selectItem(int index, bool doubleClick) override;

private:
	TexturePackScreen* m_pScreen;
};

// Classic "Select Texture Pack" screen: a scrolling list of the packs found in
// the resource_packs folder, each with its pack.png icon and pack.txt description.
class TexturePackScreen : public Screen
{
	friend class TexturePackList;

public:
	TexturePackScreen(Screen* parent);
	~TexturePackScreen();

private:
	void _findPacks();
	void _selectPack(int index);
	void _uploadIcons();
	std::string _getPackFolder() const;

public:
	void init() override;
	void render(float f) override;
	bool handleBackEvent(bool b) override;
	void _buttonClicked(Button* pButton) override;

private:
	Screen* m_pParent;
	TexturePackList* m_pList;
	std::vector<TexturePackEntry> m_packs;
	int m_selectedIndex;

public:
	Button m_btnOpenFolder;
	Button m_btnDone;
};
