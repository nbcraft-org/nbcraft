#pragma once

#include "world/item/ItemStack.hpp"
#include "Container.hpp"

class Player;

class ResultContainer : public Container
{
public:
    ResultContainer();
    virtual ~ResultContainer();

    uint16_t getContainerSize() const override;
    ItemStack& getItem(StackID stackId) override;

    std::string getName() const override;

    ItemStack removeItem(StackID stackId, int amount) override;
    void setItem(StackID stackId, const ItemStack& item) override;

    void setContainerChanged(StackID stackId) override;
    bool stillValid(Player* player) const override;

private:
    ItemStack m_item;
};
