#pragma once

#include <set>
#include <vector>
#include "GameMods.hpp"
#include "world/inventory/Container.hpp"
#include "world/item/ItemStack.hpp"
#include "world/entity/Player.hpp"
#include "world/gamemode/GameType.hpp"
#include "nbt/ListTag.hpp"

class Entity;
class Player; // in case we're included from Player.hpp


#define C_POP_TIME_DURATION (5)
#define C_MAX_HOTBAR_ITEMS (9)
#define C_NUM_INVENTORY_SLOTS (36)
#define C_NUM_ARMOR_SLOTS (4)

class Inventory : public Container
{
private:
	typedef std::set<ContainerContentChangeListener*> ContentChangeListeners;
public:
	Inventory(Player*);
	virtual ~Inventory();
	void prepareCreativeInventory();
	void prepareSurvivalInventory();

	Size getContainerSize() const override;

	void clear();
	void replace(const std::vector<ItemStack>& items);

	void addCreativeItem(int itemID, int auxValue = 0);
	void addTestItem(int itemID, int amount, int auxValue = 0);

	bool hasUnlimitedResource(const ItemStack& item) const;

	bool add(ItemStack& item);
    void tick();

	ItemStack& getItem(StackID stackId) override;
	ItemStack& getArmor(Item::EquipmentSlot slotNo);
	ItemStack& getSelectedItem();
	int getSelectedItemId();

	void setItem(StackID stackId, const ItemStack& item) override;
	void setSelectedItem(const ItemStack& item);
	ItemStack removeItem(StackID stackId, int count) override;
	bool removeResource(int id);

	void setCarried(const ItemStack& item);
	ItemStack& getCarried();

	void pickItem(int itemID, int data, int maxHotBarSlot);
	void selectItem(int itemID, int maxHotBarSlot);
	void swapItems(StackID stackIdA, StackID stackIdB);
	void selectSlot(StackID stackId);

	int getAttackDamage(Entity*);

	int getArmorValue() const;

	void hurtArmor(int amount);

	void dropAll(bool onlyClearContainer = false);

	void save(ListTag& tag) const;
	void load(const ListTag&);

	bool contains(const ItemStack&) const;

	StackID getSelectedSlotNo() const { return m_selectedStackId; }

	// v0.2.0 name alias
	ItemStack& getSelected() { return getSelectedItem(); }

	std::string getName() const override
	{
		//@TODO: Language
		return "Inventory";
	}

	void setContainerChanged(StackID stackId) override;
	void addContentChangeListener(ContainerContentChangeListener* listener) override;
	void removeContentChangeListener(ContainerContentChangeListener* listener) override;

	bool stillValid(Player* player) const override { return true; }
	
private:
	GameType _getGameMode() const;
	int getSlotWithRemainingSpace(const ItemStack& item);
	int getFreeSlot();
	int addResource(const ItemStack& item);
	int getSlot(int id);

public:
	Player* m_pPlayer;
	StackID m_selectedStackId;

private:
	ItemStack m_carried;

	std::vector<ItemStack> m_items;
	std::vector<ItemStack> m_armor;
	ContentChangeListeners m_contentChangeListeners;
};
