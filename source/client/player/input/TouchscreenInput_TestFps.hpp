/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "IMoveInput.hpp"
#include "client/gui/GuiComponent.hpp"
#include "RectangleArea.hpp"
#include "PolygonArea.hpp"
#include "TouchAreaModel.hpp"

class Minecraft;
class Options;

class TouchscreenInput_TestFps : public IMoveInput, public GuiComponent
{
public:
	TouchscreenInput_TestFps(Minecraft*, Options*);

	// IMoveInput
	void releaseAllKeys() override;
	void setKey(UserActionID, bool eventKeyState) override;
	void setScreenSize(int width, int height) override;
	void tick(Player*) override;
	void render(float f) override;

	RectangleArea getRectangleArea();
	RectangleArea getSneakArea();
	bool isButtonDown(int key);
#ifdef ENH_NEW_TOUCH_CONTROLS
	void setSneakExcludeRef(RectangleArea* pRect);
#endif

private:
	RectangleArea m_rectArea;
	bool field_30[10];
	Options* m_pOptions;
	bool m_bForwardBeingHeld;
	bool m_bJumpBeingHeld;
	TouchAreaModel m_touchAreaModel;
	Minecraft* m_pMinecraft;
	PolygonArea* m_pAreaLeft;
	PolygonArea* m_pAreaRight;
	PolygonArea* m_pAreaForward;
	PolygonArea* m_pAreaBackward;
	PolygonArea* m_pAreaJump;
#ifdef ENH_NEW_TOUCH_CONTROLS
	PolygonArea* m_pAreaForwardLeft;
	PolygonArea* m_pAreaForwardRight;
	PolygonArea* m_pAreaSneak;
	PolygonArea* m_pAreaFlyUp;
	PolygonArea* m_pAreaFlyDown;
	float m_fButtonSize;
	float m_fRightSideX;
	float m_fMiddleY;
	RectangleArea* m_pSneakExcludeRef;
#endif
	bool field_6C[10];
	bool m_bSneakBeingHeld;
	float m_fLastSneakTapTime;
	float m_fLastSneakToggleTime;
	bool m_bPersistentSneak;
	RectangleArea m_rightRectArea;
};

