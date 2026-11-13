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
	bool isAllowed(int slot);

public:
    void init() override;

    void render(float a) override;
    void tick() override;
	void removed() override;
	void handleUserAction(const ActionInfo&) override;
	bool isPauseScreen() override;

    bool addItem(const InventoryPane_Pocket& pane, int id) override;
    std::vector<const ItemStack*> getItems(const InventoryPane_Pocket& pane) override;

private:
    Player* m_pPlayer;
    InventoryPane_Pocket* m_inventoryPane;
    THeader_Pocket m_header;
    ImageButton m_btnExit;
    Button m_btnCraft;
    Button m_btnMenu;
    IntRectangle m_clippingArea;
};
