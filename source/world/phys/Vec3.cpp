/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include <cfloat>

#include "Vec3.hpp"
#include "world/level/TilePos.hpp"

const Vec3 Vec3::ZERO   = Vec3(0, 0, 0), Vec3::ONE        = Vec3(1, 1, 1);
const Vec3 Vec3::UNIT_X = Vec3(1, 0, 0), Vec3::NEG_UNIT_X = Vec3(-1, 0, 0);
const Vec3 Vec3::UNIT_Y = Vec3(0, 1, 0), Vec3::NEG_UNIT_Y = Vec3(0, -1, 0);
const Vec3 Vec3::UNIT_Z = Vec3(0, 0, 1), Vec3::NEG_UNIT_Z = Vec3(0, 0, -1);
const Vec3 Vec3::MIN = Vec3(FLT_MIN, FLT_MIN, FLT_MIN), Vec3::MAX = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
