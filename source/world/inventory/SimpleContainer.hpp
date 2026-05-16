#pragma once

#include <vector>

#include "Container.hpp"
#include "nbt/CompoundTag.hpp"

class SimpleContainer : public Container
{
public:
	SimpleContainer(Size size, const std::string& name);

public:
	Size getContainerSize() const override;
	ItemStack& getItem(StackID index) override;
	ItemStack removeItem(StackID index, int count) override;
	void setItem(StackID index, const ItemStack& item) override;
	std::string getName() const override;
	void setContainerChanged(StackID stackId) override;
	bool stillValid(Player* player) const override;
	void addContentChangeListener(ContainerContentChangeListener* listener) override;
	void addSizeChangeListener(ContainerSizeChangeListener* listener) override;
	void removeContentChangeListener(ContainerContentChangeListener* listener) override;
	void removeSizeChangeListener(ContainerSizeChangeListener* listener) override;

public:
	virtual void clear();
	virtual void load(const CompoundTag& tag);
	virtual void save(CompoundTag& tag) const;

protected:
	ContentChangeListeners m_contentChangeListeners;
	SizeChangeListeners m_sizeChangeListeners;
	std::vector<ItemStack> m_items;
	std::string m_name;
};