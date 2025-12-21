/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
// copied from wilyicaro's PEtoLE
#pragma once

#include "Model.hpp"

class SquidModel : public Model
{
public:
	SquidModel();
	~SquidModel();
	void render(float, float, float, float, float, float) override;
	void setupAnim(float, float, float, float, float, float) override;

private:
	ModelPart m_body;
	ModelPart* m_tentacles[8];
};
