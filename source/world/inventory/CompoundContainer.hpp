#pragma once

#include <set>
#include <string>
#include "Container.hpp"
#include "ContainerContentChangeListener.hpp"

class CompoundContainer : public Container
{
private:
    class ChildListener;

public:
    CompoundContainer(const std::string& name, Container* c1, Container* c2);
    ~CompoundContainer() override;

public:
    uint16_t getContainerSize() const override;
    std::string getName() const override;
    ItemStack& getItem(StackID index) override;
    ItemStack removeItem(StackID index, int count) override;
    void setItem(StackID index, const ItemStack& item) override;
    int getMaxStackSize() override;
    void setContainerChanged(StackID stackId) override;
    bool stillValid(Player* player) const override;

    void addContentChangeListener(ContainerContentChangeListener* listener) override;
    void removeContentChangeListener(ContainerContentChangeListener* listener) override;

private:
    std::string m_name;
    Container* m_pLeftContainer;
    Container* m_pRightContainer;
    ContentChangeListeners m_contentChangeListeners;
    ChildListener* m_pLeftListener;
    ChildListener* m_pRightListener;
};
