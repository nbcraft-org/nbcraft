#include "world/entity/Player.hpp"
#include "world/inventory/ContainerListener.hpp"

class ServerPlayer : public Player, public ContainerListener
{
public:
	ServerPlayer(Level* pLevel, GameType playerGameType);
	~ServerPlayer();

protected:
	void _nextContainerCounter();

public:
	void tick() override;
	void startCrafting(const TilePos& pos) override;
	void openContainer(Container* container) override;
	void closeContainer() override;
	void openFurnace(FurnaceTileEntity* tileEntity);
	void take(Entity* pEnt, int count) override;
	void checkFallDamage(float ya, bool onGround) override;
	bool interpolateOnly() const override { return true; }

	void refreshContainer(ContainerMenu* menu, const std::vector<ItemStack>& items) override;
	void slotChanged(ContainerMenu* menu, Container::SlotID slotId, Slot* slot, ItemStack& item, bool isResultSlot) override;
	void setContainerData(ContainerMenu* menu, int id, int value) override;

	void doCloseContainer();
	void setContainerMenu(ContainerMenu* menu);
	void doCheckFallDamage(float ya, bool onGround);

private:
	int m_lastHealth;
	int m_containerId;
};
