#include "CreativeMenu_Pocket.hpp"
#include "common/Util.hpp"
#include "SimpleContainer.hpp"
#include "Slot.hpp"

static SimpleContainer* container = nullptr;

CreativeMenu_Pocket::CreativeMenu_Pocket()
{
    if (!container)
    {
        // we're letting this leak, there's just one and the OS will get it anyways
        container = new SimpleContainer(creativeItems.size(), Util::EMPTY_STRING);

        // we just shotgun-blast all of the items into the static Container, and the InventoryPane handles the rest
        for (size_t i = 0; i < creativeItems.size(); i++)
        {
            container->setItem(i, creativeItems[i]);
        }
    }

    Container::Size size = container->getContainerSize();
    for (Container::StackID stackId = 0; stackId < size; stackId++)
    {
        // we're adding this to the INVENTORY group so it'll be visible in the Pocket inventory
        addSlot(new Slot(container, stackId, Slot::INVENTORY));
    }
}
