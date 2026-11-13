#include "BaseContainer.hpp"
#include "ContainerContentChangeListener.hpp"
#include "ContainerSizeChangeListener.hpp"

void BaseContainer::setContainerChanged(StackID stackId)
{
    for (ContentChangeListeners::iterator it = m_contentChangeListeners.begin(); it != m_contentChangeListeners.end(); it++)
    {
        ContainerContentChangeListener* pListener = *it;
        pListener->containerContentChanged(this, stackId);
    }
}

bool BaseContainer::stillValid(Player& player) const
{
    return true;
}

void BaseContainer::addContentChangeListener(ContainerContentChangeListener* listener)
{
    m_contentChangeListeners.insert(listener);
}

void BaseContainer::addSizeChangeListener(ContainerSizeChangeListener* listener)
{
    m_sizeChangeListeners.insert(listener);
}

void BaseContainer::removeContentChangeListener(ContainerContentChangeListener* listener)
{
    m_contentChangeListeners.erase(listener);
}

void BaseContainer::removeSizeChangeListener(ContainerSizeChangeListener* listener)
{
    m_sizeChangeListeners.erase(listener);
}
