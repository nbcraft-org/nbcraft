/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "AppPlatform.hpp"
#include "AppPlatformListener.hpp"

class App : public AppPlatformListener
{
protected:
	App()
	{
		m_bWantToQuit = false;
	}

public:
	virtual bool handleBack(bool);
	virtual void init();
	virtual void update();
	virtual void sizeUpdate(int newWidth, int newHeight);
	virtual void setTextboxText(const std::string& text);

	void destroy();
	void draw();
	void loadState(void*, int);
	void quit();
	void saveState(void**, int);
	bool wantToQuit();

public:
	bool m_bWantToQuit;

	// don't know what these are ...
	int field_8;
	int field_C;
	int field_10;
};

