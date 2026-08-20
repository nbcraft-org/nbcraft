/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#include <stdexcept>
#include "RenderChunk.hpp"
#include "common/Util.hpp"
#include "client/renderer/renderer/RenderMaterialGroup.hpp"
#include "renderer/ShaderConstants.hpp"

bool RenderChunk::_isUnderwater;
mce::MaterialPtr RenderChunk::fogMaterialMap[10];
mce::MaterialPtr RenderChunk::materialMap[10];
mce::MaterialPtr RenderChunk::fadingSeasonsAlphaChunkMaterial;
mce::MaterialPtr RenderChunk::fadingSeasonsChunksMaterial;
mce::MaterialPtr RenderChunk::fadingChunksMaterial;

void RenderChunk::_init()
{
	m_lastRebuilt = 0.0f;
}

void RenderChunk::_init(RenderChunk& other)
{
    _move(other);
    _init();
}

RenderChunk::RenderChunk(const TilePos& pos, mce::Mesh& mesh, float lastRebuilt)
	: m_lastRebuilt(lastRebuilt)
	, m_pos(pos)
	, m_mesh(mesh)
{
	//_init();
}

const mce::MaterialPtr& RenderChunk::_chooseMaterial(TerrainLayer layer, bool fog)
{
#ifdef _DEBUG
	if (layer < TERRAIN_LAYERS_MIN || layer > TERRAIN_LAYERS_MAX)
		throw std::out_of_range("Invalid TerrainLayer");
#endif

	mce::MaterialPtr* map = fog ? fogMaterialMap : materialMap;

	return map[layer] ? map[layer] : materialMap[layer];
}

const mce::MaterialPtr& RenderChunk::_chooseMaterial(TerrainLayer layer, bool fog, float a)
{
	float diff = a - m_lastRebuilt;
    
	if (diff < 1.2f)
	{
		currentShaderColor.r = (1.2f - diff) * 0.2f;
		switch (layer)
		{
            case TERRAIN_LAYER_SEASONS_FAR:
                return fadingSeasonsChunksMaterial;
            case TERRAIN_LAYER_SEASONS_FAR_ALPHATEST:
                return fadingSeasonsAlphaChunkMaterial;
            default:
                return fadingChunksMaterial;
		}
	}
    
	return _chooseMaterial(layer, fog);
}

void RenderChunk::_move(RenderChunk& other)
{
	m_lastRebuilt = other.m_lastRebuilt;
    m_pos = other.m_pos;
    m_mesh = other.m_mesh;
}

void RenderChunk::render(TerrainLayer layer, bool fog)
{
	currentShaderColor = Color::WHITE;
	currentShaderDarkColor = Color::WHITE;
	m_mesh.render(_chooseMaterial(layer, fog));
}

void RenderChunk::render(TerrainLayer layer, bool fog, float a)
{
	currentShaderColor = Color::WHITE;
	currentShaderDarkColor = Color::WHITE;
	m_mesh.render(_chooseMaterial(layer, fog, a));
}

void RenderChunk::reset()
{
	m_mesh.reset();
}

void RenderChunk::_InitLayers(mce::MaterialPtr* materials, const std::string& suffix)
{
	materials[TERRAIN_LAYER_OPAQUE]                = GET_MATERIAL_PTR(switchable, "terrain_opaque"            + suffix);
	materials[TERRAIN_LAYER_OPAQUE_SEASONS]        = GET_MATERIAL_PTR(switchable, "terrain_opaque_seasons"    + suffix);
	materials[TERRAIN_LAYER_BLEND]                 = GET_MATERIAL_PTR(switchable, "terrain_blend"             + suffix);
	materials[TERRAIN_LAYER_ALPHATEST]             = GET_MATERIAL_PTR(switchable, "terrain_alpha"             + suffix);
	materials[TERRAIN_LAYER_ALPHATEST_SINGLE_SIDE] = GET_MATERIAL_PTR(switchable, "terrain_alpha_single_side" + suffix);
	materials[TERRAIN_LAYER_ALPHATEST_SEASONS]     = GET_MATERIAL_PTR(switchable, "terrain_alpha_seasons"     + suffix);
	materials[TERRAIN_LAYER_DOUBLE_SIDED]          = GET_MATERIAL_PTR(switchable, "terrain_doubleside"        + suffix);
	materials[TERRAIN_LAYER_FAR]                   = GET_MATERIAL_PTR(switchable, "terrain_far"               + suffix);
	materials[TERRAIN_LAYER_SEASONS_FAR]           = GET_MATERIAL_PTR(switchable, "terrain_seasons_far"       + suffix);
	materials[TERRAIN_LAYER_SEASONS_FAR_ALPHATEST] = GET_MATERIAL_PTR(switchable, "terrain_seasons_far_alpha" + suffix);
}

void RenderChunk::InitMaterials()
{
	fadingChunksMaterial            = GET_MATERIAL_PTR(switchable, "terrain_fading_in");
	fadingSeasonsChunksMaterial     = GET_MATERIAL_PTR(switchable, "terrain_seasons_fading_in");
	fadingSeasonsAlphaChunkMaterial = GET_MATERIAL_PTR(switchable, "terrain_seasons_fading_in_alpha");

	_InitLayers(materialMap, Util::EMPTY_STRING);
	_InitLayers(fogMaterialMap, "_fog");
}

void RenderChunk::SetUnderwater(bool isUnderwater)
{
	if (_isUnderwater == isUnderwater)
		return;

	_isUnderwater = isUnderwater;

	std::string materialName = isUnderwater ? "terrain_blend_below" : "terrain_blend";
	materialMap[TERRAIN_LAYER_BLEND] = GET_MATERIAL_PTR(switchable, materialName);

	materialName = isUnderwater ? "terrain_blend_below_fog" : "terrain_blend_fog";
	fogMaterialMap[TERRAIN_LAYER_BLEND] = GET_MATERIAL_PTR(switchable, materialName);
}
