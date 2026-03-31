#include "FurnaceScreen.hpp"
#include "world/inventory/FurnaceMenu.h"
#include "client/app/Minecraft.hpp"

FurnaceScreen::FurnaceScreen(Inventory* inventory, FurnaceTileEntity* furnace)
    : ContainerScreen(new FurnaceMenu(inventory, furnace)), m_pFurnace(furnace)
{
}

void FurnaceScreen::_renderLabels() {
    // Draw text using the built-in font renderer (Text, X, Y, Hex Color)
    m_pFont->draw("Furnace", 64 , 6, 0x404040);
}

void FurnaceScreen::_renderBg(float partialTick) {
    // 1. Bind the texture file (using the exact pointer your compiler suggested)
    m_pMinecraft->m_pTextures->loadAndBindTexture("gui/furnace.png");

    // 2. Define the exact pixel size of the texture locally
    int xSize = 176;
    int ySize = 166;

    // 3. Center the UI on the screen
    int x = (m_width - xSize) / 2;
    int y = (m_height - ySize) / 2;

    // 4. Draw the main background
    // Notice how we pass xSize and ySize twice (Destination Width/Height, then Source Width/Height)
    blit(x, y, 0, 0, xSize, ySize, xSize, ySize);

    // 5. Draw the animated Fire (Fuel)
    if (m_pFurnace->isBurning()) {
        int fireHeight = 12; // Maximum height of the fire icon
        if (m_pFurnace->currentItemBurnTime > 0) {
            // Scale the fire down as the fuel burns away
            fireHeight = (m_pFurnace->furnaceBurnTime * 12) / m_pFurnace->currentItemBurnTime;
        }

        // Draw the fire! (Again, we duplicate the width '14' and height 'fireHeight + 2')
        blit(x + 56, y + 36 + 12 - fireHeight, 176, 12 - fireHeight, 14, fireHeight + 2, 14, fireHeight + 2);
    }

    // 6. Draw the animated Progress Arrow (Cook Time)
    // The arrow is 24 pixels wide, and a full smelt takes 200 ticks
    int arrowWidth = (m_pFurnace->furnaceCookTime * 24) / 200;

    // Draw the arrow! (Duplicating the width 'arrowWidth' and height '16')
    blit(x + 79, y + 34, 176, 14, arrowWidth, 16, arrowWidth, 16);
}

SlotDisplay FurnaceScreen::_createSlotDisplay(const Slot& slot) {

    if (slot.m_pContainer == m_pFurnace) {
        const int id = slot.m_slot; // Slot 0, 1, or 2
        if (id == 0) return SlotDisplay(56, 17);  // Top Input Slot
        if (id == 1) return SlotDisplay(56, 53);  // Bottom Fuel Slot
        if (id == 2) return SlotDisplay(116, 35); // Right Output Slot
    }
    // Otherwise, it belongs to the Player's Inventory
    else {
        const int id = slot.m_slot;
        if (id < 9) {
            // The Hotbar (Bottom row)
            return SlotDisplay(8 + id * 18, 142);
        } else {
            // The Main Backpack (3 rows of 9)
            int gridX = (id - 9) % 9;
            int gridY = (id - 9) / 9;
            return SlotDisplay(8 + gridX * 18, 84 + gridY * 18);
        }
    }
    return SlotDisplay(0, 0); // Safety fallback
}