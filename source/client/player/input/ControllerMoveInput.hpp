#pragma once

#include "KeyboardInput.hpp"

class ControllerMoveInput : public KeyboardInput
{
private:
	bool field_20;
	bool field_21;
	bool m_bFlying;

public:
	ControllerMoveInput(Options *options);

	void tick(Player *player) override;
	void setKey(UserActionID, bool eventKeyState) override;
	void releaseAllKeys() override;
};

