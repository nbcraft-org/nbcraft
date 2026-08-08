/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
#include "PigRenderer.hpp"
#include "world/entity/Pig.hpp"

PigRenderer::PigRenderer(Model* pModel, Model* pArmor, float f) : MobRenderer(pModel, f)
{
	setArmor(pArmor);
}

PigRenderer::~PigRenderer()
{}

bool PigRenderer::prepareArmor(const Mob& mob, int a, float b)
{
	bindTexture("mob/saddle.png");
	return a == 0 && ((const Pig&)mob).hasSaddle();
}

