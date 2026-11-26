#include "TexturePackScreen.hpp"

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include "common/Util.hpp"
#include "common/Utils.hpp"
#include "client/resources/Resource.hpp"
#include "client/resources/ResourceLocation.hpp"
#include "client/resources/ResourcePackRepository.hpp"

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

// Opening a folder in the file manager only makes sense on desktop systems.
#if (defined(_WIN32) && !defined(_XBOX)) || (defined(__APPLE__) && TARGET_OS_OSX) || (defined(__linux__) && !defined(__ANDROID__) && !defined(__SWITCH__))
#define CAN_OPEN_PACK_FOLDER 1
#else
#define CAN_OPEN_PACK_FOLDER 0
#endif

#define C_PACK_ICON_SIZE (32)

// TexturePackList

TexturePackList::TexturePackList(Minecraft* pMinecraft, TexturePackScreen* pScreen, int width, int height, int y0, int y1, int itemHeight) :
	ScrolledSelectionList(pMinecraft, width, height, y0, y1, itemHeight),
	m_pScreen(pScreen)
{
}

int TexturePackList::getNumberOfItems()
{
	return int(m_pScreen->m_packs.size());
}

bool TexturePackList::isSelectedItem(int i)
{
	return m_pScreen->m_selectedIndex == i;
}

void TexturePackList::renderBackground(float f)
{
}

void TexturePackList::renderItem(int index, int x, int y, int height, const MenuPointer& pointer, Tesselator& t)
{
	const TexturePackEntry& pack = m_pScreen->m_packs[index];
	Font& font = *m_pMinecraft->m_pFont;

	// pack icon
	TextureData* pIcon = nullptr;
	if (!pack.iconTexture.empty() && m_pMinecraft->m_pTextures->hasTexture(pack.iconTexture))
		pIcon = m_pMinecraft->m_pTextures->loadAndBindTexture(pack.iconTexture, false);

	if (pIcon)
	{
		float x0 = float(x), y0 = float(y);
		float x1 = x0 + C_PACK_ICON_SIZE, y1 = y0 + C_PACK_ICON_SIZE;
		t.begin(4);
		t.color(0xFFFFFF);
		t.vertexUV(x0, y1, 0.0f, 0.0f, 1.0f);
		t.vertexUV(x1, y1, 0.0f, 1.0f, 1.0f);
		t.vertexUV(x1, y0, 0.0f, 1.0f, 0.0f);
		t.vertexUV(x0, y0, 0.0f, 0.0f, 0.0f);
		t.draw(m_materials.ui_texture_and_color);
	}
	else
	{
		// no icon: a plain dark square, like the "unknown pack" icon
		fill(x, y, x + C_PACK_ICON_SIZE, y + C_PACK_ICON_SIZE, 0xFF303030);
		drawCenteredString(font, "?", x + C_PACK_ICON_SIZE / 2, y + 12, 0x808080);
	}

	int textX = x + C_PACK_ICON_SIZE + 2;
	drawString(font, pack.title, textX, y + 1, 0xFFFFFF);
	drawString(font, pack.desc1, textX, y + 12, 0x808080);
	drawString(font, pack.desc2, textX, y + 22, 0x808080);
}

void TexturePackList::selectItem(int index, bool doubleClick)
{
	m_pScreen->_selectPack(index);
}

// TexturePackScreen

TexturePackScreen::TexturePackScreen(Screen* parent) :
	m_pParent(parent),
	m_pList(nullptr),
	m_selectedIndex(0),
	m_btnOpenFolder("Open texture pack folder"),
	m_btnDone("Done")
{
	m_bDeletePrevious = false;
}

TexturePackScreen::~TexturePackScreen()
{
	SAFE_DELETE(m_pList);
}

static bool _ComparePacks(const TexturePackEntry& a, const TexturePackEntry& b)
{
	return a.name < b.name;
}

static void _ReadPackText(const std::string& path, std::string& line1, std::string& line2)
{
	std::string text;
	if (!Resource::load(ResourceLocation(ResourceLocation::APP_PACKAGE, path), text) &&
		!Resource::load(ResourceLocation(ResourceLocation::EXTERNAL_DIR, path), text))
		return;

	std::istringstream stream(text);
	std::string line;
	int n = 0;
	while (std::getline(stream, line) && n < 2)
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		(n == 0 ? line1 : line2) = line;
		n++;
	}
}

std::string TexturePackScreen::_getPackFolder() const
{
	ResourceLocation location(ResourceLocation::EXTERNAL_DIR, ResourcePackRepository::RESOURCE_PACKS_PATH);
	return location.getFullPath();
}

void TexturePackScreen::_findPacks()
{
	m_packs.clear();

	TexturePackEntry def;
	def.name = "";
	def.title = "Default";
	def.desc1 = "The default look of " C_GAME_NAME;
	def.iconLocation = ResourceLocation(ResourceLocation::APP_PACKAGE, ResourcePackRepository::RESOURCE_PACK_ICON_PATH);
	m_packs.push_back(def);

	std::vector<std::string> found;
	std::vector<ResourceLocation::FileSystem> foundIn;

	// Packs can live in the built-in assets or in the external storage directory
	const ResourceLocation::FileSystem fileSystems[] = { ResourceLocation::APP_PACKAGE, ResourceLocation::EXTERNAL_DIR };
	for (size_t f = 0; f < sizeof(fileSystems) / sizeof(fileSystems[0]); f++)
	{
		ResourceLocation location(fileSystems[f], ResourcePackRepository::RESOURCE_PACKS_PATH);
		std::string dirPath = location.getFullPath();

		DIR* dir = opendir(dirPath.c_str());
		if (!dir)
			continue;

		while (dirent* de = readdir(dir))
		{
			std::string name = de->d_name;
			if (name.empty() || name[0] == '.' || name == C_DEFAULT_RESOURCE_PACK)
				continue;

			std::string fullPath = dirPath + "/" + name;
			if (!isDirectory(fullPath.c_str()))
				continue;

			if (std::find(found.begin(), found.end(), name) == found.end())
			{
				found.push_back(name);
				foundIn.push_back(fileSystems[f]);
			}
		}

		closedir(dir);
	}

	for (size_t i = 0; i < found.size(); i++)
	{
		TexturePackEntry entry;
		entry.name = found[i];
		entry.title = found[i];

		std::string packPath = ResourcePackRepository::RESOURCE_PACKS_PATH + "/" + found[i] + "/";
		_ReadPackText(packPath + "pack.txt", entry.desc1, entry.desc2);
		entry.iconLocation = ResourceLocation(foundIn[i], packPath + ResourcePackRepository::RESOURCE_PACK_ICON_PATH);

		m_packs.push_back(entry);
	}

	// sort the packs (but not the default one) by name
	std::sort(m_packs.begin() + 1, m_packs.end(), _ComparePacks);

	for (size_t i = 0; i < m_packs.size(); i++)
		m_packs[i].iconTexture = "texturepack_icon/" + Util::toString(int(i)) + "/" + m_packs[i].name + ".png";
}

// Each pack's icon is read straight from that pack, then uploaded under its own name.
// Textures get cleared whenever the pack changes, so this is checked every frame.
void TexturePackScreen::_uploadIcons()
{
	Textures& textures = *m_pMinecraft->m_pTextures;

	for (size_t i = 0; i < m_packs.size(); i++)
	{
		TexturePackEntry& entry = m_packs[i];
		if (entry.iconMissing || textures.hasTexture(entry.iconTexture))
			continue;

		TextureData data = Resource::loadTexture(entry.iconLocation);
		if (data.isEmpty())
		{
			entry.iconMissing = true;
			continue;
		}

		data.m_bEnableFiltering = false;
		data.m_bWrap = false;
		textures.uploadTexture(entry.iconTexture, data);
	}
}

void TexturePackScreen::_selectPack(int index)
{
	if (index < 0 || index >= int(m_packs.size()) || index == m_selectedIndex)
		return;

	m_selectedIndex = index;

	// Like the original, the pack is applied as soon as it is clicked
	Options* pOptions = m_pMinecraft->getOptions();
	pOptions->setActiveResourcePack(m_packs[index].name);
	pOptions->save();
	m_pMinecraft->reloadTextures();
}

void TexturePackScreen::init()
{
	_findPacks();

	std::string active = m_pMinecraft->getOptions()->getActiveResourcePack();
	m_selectedIndex = 0;
	for (size_t i = 0; i < m_packs.size(); i++)
	{
		if (m_packs[i].name == active)
			m_selectedIndex = int(i);
	}

	SAFE_DELETE(m_pList);
	m_pList = new TexturePackList(m_pMinecraft, this, m_width, m_height, 32, m_height - 58 + 4, 36);
	_addElement(*m_pList, false);

	m_btnOpenFolder.m_width = m_btnDone.m_width = 150;
	m_btnOpenFolder.m_height = m_btnDone.m_height = 20;
	m_btnOpenFolder.m_xPos = m_width / 2 - 154;
	m_btnDone.m_xPos = m_width / 2 + 4;
	m_btnOpenFolder.m_yPos = m_btnDone.m_yPos = m_height - 48;

	m_btnOpenFolder.setEnabled(CAN_OPEN_PACK_FOLDER != 0);

	_addElement(m_btnOpenFolder);
	_addElement(m_btnDone);
}

void TexturePackScreen::render(float f)
{
	_uploadIcons();
	renderBackground();
	Screen::render(f);

	drawCenteredString(*m_pFont, "Select Texture Pack", m_width / 2, 16, 0xFFFFFF);
	drawCenteredString(*m_pFont, "(Place texture pack folders here)", m_width / 2 - 77, m_height - 26, 0x808080);
}

void TexturePackScreen::_buttonClicked(Button* pButton)
{
	if (pButton->getId() == m_btnDone.getId())
	{
		handleBackEvent(false);
		return;
	}

	if (pButton->getId() == m_btnOpenFolder.getId())
	{
#if CAN_OPEN_PACK_FOLDER
		std::string folder = _getPackFolder();
		createFolderIfNotExists(folder.c_str());
#if defined(_WIN32)
		std::string command = "explorer \"" + folder + "\"";
#elif defined(__APPLE__)
		std::string command = "open \"" + folder + "\"";
#else
		std::string command = "xdg-open \"" + folder + "\" &";
#endif
		int result = system(command.c_str());
		(void)result;
#endif
		return;
	}
}

bool TexturePackScreen::handleBackEvent(bool b)
{
	if (!b)
		m_pMinecraft->setScreen(m_pParent);

	return true;
}
