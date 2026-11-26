#include "TexturePackScreen.hpp"

#include <algorithm>
#include "common/Utils.hpp"
#include "client/resources/ResourceLocation.hpp"
#include "client/resources/ResourcePackRepository.hpp"

TexturePackScreen::TexturePackScreen(Screen* parent) :
	m_pParent(parent),
	m_btnDone("Done")
{
	m_bDeletePrevious = false;
}

TexturePackScreen::~TexturePackScreen()
{
	_clearPackButtons();
}

void TexturePackScreen::_clearPackButtons()
{
	for (size_t i = 0; i < m_packButtons.size(); i++)
		delete m_packButtons[i];
	m_packButtons.clear();
}

void TexturePackScreen::_findPacks()
{
	m_packs.clear();
	m_packs.push_back(""); // default

	std::vector<std::string> found;

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
				found.push_back(name);
		}

		closedir(dir);
	}

	std::sort(found.begin(), found.end());
	m_packs.insert(m_packs.end(), found.begin(), found.end());
}

void TexturePackScreen::_updateButtonLabels()
{
	for (size_t i = 0; i < m_packButtons.size(); i++)
	{
		std::string name = m_packs[i].empty() ? "Default" : m_packs[i];
		if (m_packs[i] == m_selected)
			name = "> " + name + " <";
		m_packButtons[i]->setMessage(name);
	}
}

void TexturePackScreen::init()
{
	m_original = m_selected = m_pMinecraft->getOptions()->getActiveResourcePack();

	_findPacks();
	_clearPackButtons();

	m_btnDone.m_width = 200;
	m_btnDone.m_height = 20;
	m_btnDone.m_xPos = (m_width - m_btnDone.m_width) / 2;
	m_btnDone.m_yPos = m_height - 28;

	// as many packs as fit between the title and the Done button
	const int top = 32;
	const int spacing = 24;
	int maxButtons = (m_btnDone.m_yPos - 8 - top) / spacing;
	if (maxButtons < 1)
		maxButtons = 1;

	for (size_t i = 0; i < m_packs.size() && int(i) < maxButtons; i++)
	{
		Button* pButton = new Button(0, 0, 200, 20, "");
		pButton->m_xPos = (m_width - pButton->m_width) / 2;
		pButton->m_yPos = top + int(i) * spacing;
		m_packButtons.push_back(pButton);
		_addElement(*pButton);
	}

	_addElement(m_btnDone);
	_updateButtonLabels();
}

void TexturePackScreen::render(float f)
{
	renderBackground();
	drawCenteredString(*m_pFont, "Select Texture Pack", m_width / 2, 12, 0xFFFFFF);

	if (m_packs.size() <= 1)
		drawCenteredString(*m_pFont, "Put texture packs in the resource_packs folder", m_width / 2, 64, 0x999999);

	Screen::render(f);
}

void TexturePackScreen::_buttonClicked(Button* pButton)
{
	if (pButton->getId() == m_btnDone.getId())
	{
		handleBackEvent(false);
		return;
	}

	for (size_t i = 0; i < m_packButtons.size(); i++)
	{
		if (pButton->getId() == m_packButtons[i]->getId())
		{
			m_selected = m_packs[i];
			_updateButtonLabels();
			return;
		}
	}
}

bool TexturePackScreen::handleBackEvent(bool b)
{
	if (!b)
	{
		if (m_selected != m_original)
		{
			Options* pOptions = m_pMinecraft->getOptions();
			pOptions->setActiveResourcePack(m_selected);
			pOptions->save();
			m_pMinecraft->reloadTextures();
		}

		m_pMinecraft->setScreen(m_pParent);
	}

	return true;
}
