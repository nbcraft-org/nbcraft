#pragma once

#include "Screen.hpp"

class TabLayout : public GuiElement
{
public:
    class Navigation : public AreaNavigation
    {
    public:
        Navigation(TabLayout*);

        bool next(int& x, int& y, bool invert) override;

        bool isValid(ID) override;
    private:
        TabLayout* m_pLayout;
    };

    TabLayout(Screen* screen);
    ~TabLayout();

    GuiElement* getElement(ID) const;
    bool selectElementById(ID, bool sound = false);
    bool selectElement(GuiElement*);
    ID getIndex() const;

    virtual void onSelectElement(GuiElement*);
    void init(int x, int y, int spacing = 0, AreaNavigation::Direction organizeDir = AreaNavigation::RIGHT, bool cyclic = true);
    void organize();
    void clear();

    void startNavigation();
    bool areaNavigation(Minecraft* pMinecraft, AreaNavigation::Direction) override;
    void areaNavigation(AreaNavigation::Direction, bool cyclic = true);
    void setSelected(bool);

    bool isHovered(Minecraft*, const MenuPointer& pointer) override;
    void pressed(Minecraft*, const MenuPointer& pointer) override;
    void released(const MenuPointer& pointer) override;
    void pressed(Minecraft*) override;

    void render(Minecraft*, const MenuPointer&) override;
    void renderSelected(Minecraft*, const MenuPointer&);

public:
    Screen* m_pScreen;
    GuiElementList m_elements;
    GuiElement* m_pSelectedElement;
    GuiElement* m_pClickedElement;
    int m_spacing;
    AreaNavigation::Direction m_organizeDir;
    bool m_bCyclic;
};
