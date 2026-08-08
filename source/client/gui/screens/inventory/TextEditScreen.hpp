#pragma once

#include "client/gui/Screen.hpp"
#include "client/gui/components/Button.hpp"
#include "world/tile/entity/SignTileEntity.hpp"

class TextEditScreen : public Screen
{
public:
    TextEditScreen(SignTileEntity*);

protected:
    void _buttonClicked(Button* pButton) override;

public:
    void init() override;
    void render(float partialTick) override;
    void removed() override;
	
    void handlePointerAction(const MenuPointer& pointer, MouseButtonType button) override;
    void handleUserAction(const ActionInfo& action) override;
    void handleTextChar(char chr) override;
    void tick() override;

private:
    std::string m_title;
    int m_frame;
    int m_line;
    SignTileEntity* m_pSign;
    Button m_btnDone;
};
