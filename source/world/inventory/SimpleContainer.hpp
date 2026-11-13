#pragma once

#include <vector>

#include "BaseContainer.hpp"
#include "nbt/CompoundTag.hpp"

class SimpleContainer : public BaseContainer
{
public:
	SimpleContainer(Size size, const std::string& name);

public:
	ItemStack& getItem(StackID index) override;
	ItemStack removeItem(StackID index, int count) override;
	void setItem(StackID index, const ItemStack& item) override;
	std::string getName() const override;
	Size getContainerSize() const override;

public:
	virtual void clear();
	virtual void load(const CompoundTag& tag);
	virtual void save(CompoundTag& tag) const;

protected:
	std::vector<ItemStack> m_items;
	std::string m_name;
};