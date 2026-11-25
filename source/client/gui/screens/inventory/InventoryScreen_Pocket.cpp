#include "InventoryScreen_Pocket.hpp"
#include "renderer/ShaderConstants.hpp"
#include "world/inventory/CreativeMenu_Pocket.hpp"
#include "world/inventory/SimpleContainer.hpp"
#include "world/inventory/Slot.hpp"

static const TextureAtlasSprite SPRITE_SCREEN_FRAME = TextureAtlasSprite(130, 139, 24, 24, "gui/gui2.png", 256, 256);

InventoryScreen_Pocket::InventoryScreen_Pocket(Player* player)
    : m_pPlayer(player)
	, m_pCreativeMenu(new CreativeMenu_Pocket())
	, m_header("Select blocks")
	, m_btnExit("")
	, m_btnCraft("Craft")
	, m_btnMenu("Menu")
{
	m_inventoryPane = nullptr;
}

InventoryScreen_Pocket::~InventoryScreen_Pocket()
{
	SAFE_DELETE(m_inventoryPane);
	SAFE_DELETE(m_pCreativeMenu);
}

void InventoryScreen_Pocket::_setupPositions()
{
    m_btnCraft.m_xPos = 0;
    m_btnCraft.m_yPos = 0;
    m_btnCraft.m_width = 48;
	m_btnCraft.m_height = m_header.m_height;

    m_btnExit.m_xPos = m_width - m_btnExit.m_width;
    m_btnExit.m_yPos = 0;

    m_header.m_yPos = 0;

    if (m_pPlayer && m_pPlayer->isCreative())
    {
        m_header.m_xPos = 0;
        m_header.m_width = m_width;
    }
    else
    {
        m_header.m_xPos = m_btnCraft.m_width;
        m_header.m_width = m_width - m_btnCraft.m_width;
    }

    m_header.m_textPosX = m_width / 2;

	m_clippingArea.x = 0;
	m_clippingArea.y = 0;
	m_clippingArea.w = Minecraft::GetWidthL();
	m_clippingArea.h = Gui::GuiScale * 24.0f;
}

void InventoryScreen_Pocket::_buttonClicked(Button* pButton)
{
	if (pButton->getId() == m_btnExit.getId())
		m_pMinecraft->setScreen(nullptr);
	else if (pButton->getId() == m_btnMenu.getId())
		m_pMinecraft->getScreenChooser()->pushPauseScreen();
	/*else if (pButton->getId() == m_btnCraft.getId())
		m_pMinecraft->setScreen(new CraftAutoScreen(m_pPlayer));
	else if (pButton->getId() == m_btnArmor.getId())
		m_pMinecraft->setScreen(new ArmorScreen(m_pPlayer));*/
}

void InventoryScreen_Pocket::init()
{
    int v4 = m_width - 20;
    if (m_width - 20 < 0)
        v4 = m_width + 11;
    int v5 = v4 >> 5;
    //this->dword188 = v5;
    v5 *= 32;
    int v6 = m_width - v5;

    IntRectangle areaRect(v6 / 2, 30, v5, m_height - 56);
	ContainerMenu& containerMenu = *_getContainerMenu();

	std::vector<Slot*>& hotbarSlots = containerMenu.getSlotsByGroup(Slot::HOTBAR);
	std::vector<Slot*>& inventorySlots = containerMenu.getSlotsByGroup(Slot::INVENTORY);
	int itemSlotsCount = hotbarSlots.size() + inventorySlots.size();

	m_inventoryPane = new InventoryPane_Pocket(*this, *m_pMinecraft,
		areaRect,
        m_width,
        4.0f,
		itemSlotsCount
	);
	m_inventoryPane->m_paddingX = v6 / 2;
	//this->dword184 = itemSlotsCount;
	//this->dword180 = (itemSlotsCount - 1) / this->dword188 + 1;

	ImageDef exitImg;
	{
		exitImg.name = "gui/spritesheet.png";
		exitImg.xOffset = 0;
		exitImg.yOffset = 1;
		exitImg.width = 18.0f;
		exitImg.height = 18.0f;
		exitImg.uv.x = 60;
		exitImg.uv.y = 0;
		exitImg.uv.w = 18;
		exitImg.uv.h = 18;
		exitImg.bUseCustomUV = true;
	}
	m_btnExit.setImageDef(exitImg);

	m_btnExit.m_width = 19;
	m_btnExit.m_height = 19;
	m_btnExit.m_bScaleOnPress = false;

	_setupPositions();

	_addElement(m_header);
	_addElement(m_btnExit);
	if (!m_pPlayer->isCreative())
		_addElement(m_btnCraft);
	//_addElement(m_btnMenu);
	_addElement(*m_inventoryPane);
}

void InventoryScreen_Pocket::render(float a)
{
    Screen::render(a);

	currentShaderColor = Color::WHITE;

	IntRectangle frameRect(
		0, m_inventoryPane->m_yPos - 6,
		m_width,
		m_inventoryPane->m_height + 12
	);

	/*m_pMinecraft->m_pTextures->loadAndBindTexture("gui/itemframe.png");
	blit(
		frameRect.x, frameRect.y,
		0, 0,
		frameRect.w, frameRect.h,
		215, 256
	);*/

	blitNineSlice(*m_pMinecraft->m_pTextures, SPRITE_SCREEN_FRAME,
		frameRect.x, frameRect.y, frameRect.w, frameRect.h,
		8
	);

	/*fillGradient(
		frameRect.x, frameRect.y, frameRect.x + frameRect.w, frameRect.y + frameRect.h,
		Color(40, 36, 35, 109), Color::NIL
	);*/
}

void InventoryScreen_Pocket::tick()
{
	m_inventoryPane->tick();
}

void InventoryScreen_Pocket::removed()
{
	m_pMinecraft->m_pGui->inventoryUpdated();
}

void InventoryScreen_Pocket::handleUserAction(const ActionInfo& info)
{
	if (!_useController() && m_pMinecraft->getOptions()->isAction(AID_INVENTORY, info))
	{
		m_pMinecraft->handleBack(false);
	}
	else
	{
		Screen::handleUserAction(info);
	}
}

bool InventoryScreen_Pocket::isPauseScreen()
{
	return false;
}

bool InventoryScreen_Pocket::addItem(const InventoryPane_Pocket& pane, Container::SlotID slotId)
{
	Inventory& inventory = *m_pPlayer->m_pInventory;

	Slot* pSlot = _getContainerMenu()->getSlot(slotId);
	if (!pSlot || !pSlot->hasItem())
		return false;

	Container::StackID stackId;

	// @HACK: we're not using the linked slots properly here, becuase we can't. the creative items aren't really in our inventory until we select them
	if (m_pPlayer->isCreative())
	{
		ItemStack& itemStack = pSlot->getItem();

		if (inventory.getSelectedItem() != itemStack)
		{
			stackId = inventory.tryGetLinkedSlot(inventory.getSelectedSlotNo());
			stackId++;
			if (stackId > inventory.getNumHotbarSlots())
				stackId = 0;

			inventory.moveToSelectionSlot(0, stackId, true);
			inventory.setSelectedItem(itemStack);
		}
	}
	else
	{
		stackId = pSlot->m_stackId;
		inventory.moveToSelectionSlot(0, stackId, true);
	}

	
	inventory.selectSlot(0);
	m_pMinecraft->m_pSoundEngine->playUI("random.pop", 1.0f, 0.3f);
	m_pMinecraft->m_pGui->flashSlot(0);

	return true;
}

bool InventoryScreen_Pocket::isAllowed(Container::SlotID slotId)
{
	return slotId >= 0 && slotId < int(_getContainerMenu()->m_slots.size());
}

ContainerMenu* InventoryScreen_Pocket::getContainerMenu(const InventoryPane_Pocket& pane)
{
	return _getContainerMenu();
}
