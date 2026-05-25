/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include <cfloat>

#include "Vec2.hpp"

const Vec2 Vec2::ZERO = Vec2(0, 0), Vec2::ONE = Vec2(1, 1);
const Vec2 Vec2::UNIT_X = Vec2(1, 0), Vec2::NEG_UNIT_X = Vec2(-1, 0);
const Vec2 Vec2::UNIT_Y = Vec2(0, 1), Vec2::NEG_UNIT_Y = Vec2(0, -1);
const Vec2 Vec2::MIN = Vec2(FLT_MIN, FLT_MIN), Vec2::MAX = Vec2(FLT_MAX, FLT_MAX);
