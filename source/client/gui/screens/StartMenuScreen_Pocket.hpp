#pragma once

#include "StartMenuScreen.hpp"

class StartMenuScreen_Pocket : public StartMenuScreen
{
public:
	StartMenuScreen_Pocket();
	~StartMenuScreen_Pocket();

protected:
	void _updateLicense();
	void _setup() override;
	void _setupPositions();
};
