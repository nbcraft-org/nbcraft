#pragma once

#include "Container.hpp"

class BaseContainer : public Container
{
public:
	void setContainerChanged(StackID stackId) override;
	bool stillValid(Player& player) const override;
	void addContentChangeListener(ContainerContentChangeListener* listener) override;
	void addSizeChangeListener(ContainerSizeChangeListener* listener) override;
	void removeContentChangeListener(ContainerContentChangeListener* listener) override;
	void removeSizeChangeListener(ContainerSizeChangeListener* listener) override;

protected:
	ContentChangeListeners m_contentChangeListeners;
	SizeChangeListeners m_sizeChangeListeners;
};