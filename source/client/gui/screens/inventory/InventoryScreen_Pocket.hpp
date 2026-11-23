#pragma once

#include "client/gui/Screen.hpp"
#include "client/gui/components/ImageButton.hpp"
#include "client/gui/components/InventoryPane_Pocket.hpp"
#include "client/gui/components/THeader_Pocket.hpp"
#include "world/entity/Player.hpp"

class InventoryScreen_Pocket : public Screen, public InventoryPane_Pocket::ICallback
{
public:
    InventoryScreen_Pocket(Player* player);
	~InventoryScreen_Pocket();

protected:
    void _setupPositions() override;
	void _buttonClicked(Button*) override;

public:
    void init() override;

    void render(float a) override;
    void tick() override;
	void removed() override;
	void handleUserAction(const ActionInfo&) override;
	bool isPauseScreen() override;

    // InventoryPane_Pocket::ICallback members
    bool addItem(const InventoryPane_Pocket& pane, Container::SlotID slotId) override;
    bool isAllowed(Container::SlotID slotId) override;
    ContainerMenu* getContainerMenu(const InventoryPane_Pocket& pane) override;

private:
    Player* m_pPlayer;
    ContainerMenu* m_pMenu;
    InventoryPane_Pocket* m_inventoryPane;
    THeader_Pocket m_header;
    ImageButton m_btnExit;
    Button m_btnCraft;
    Button m_btnMenu;
    IntRectangle m_clippingArea;
};
