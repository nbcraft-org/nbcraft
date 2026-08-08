#include "PistonRenderer.hpp"
#include "../Lighting.hpp"
#include "world/tile/PistonHeadTile.hpp"
#include "world/tile/PistonBaseTile.hpp"
#include "../RenderChunk.hpp"
#include "renderer/RenderContextImmediate.hpp"

void PistonRenderer::renderTyped(PistonMovingTileEntity* piston, const Vec3& pos, float partialTicks)
{
	Tile* tile = Tile::tiles[piston->getTileId()];
	if (tile && piston->getProgress(partialTicks) < 1.0f)
	{
		mce::RenderContext& renderContext = mce::RenderContextImmediate::get();
		m_tileRenderer.setTileSource(piston->m_pLevel);
		Tesselator& t = Tesselator::instance;
		bindTexture(C_TERRAIN_NAME);
		Lighting::turnOff();
		if (m_tileRenderer.useAmbientOcclusion())
		{
			renderContext.setShadeMode(mce::SHADE_MODE_SMOOTH);
		}

		t.begin(64);
		Vec3 off = piston->getOff(partialTicks);
		t.setOffset(pos.x - piston->m_pos.x + off.x, pos.y - piston->m_pos.y + off.y, pos.z - piston->m_pos.z + off.z);
		t.color(Color::WHITE);
		if (tile == Tile::pistonHead && piston->getProgress(partialTicks) < 0.5f)
		{
			m_tileRenderer.tesselateHeadPistonInWorldNoCulling(tile, piston->m_pos, false);
		}
		else if (piston->isSourcePiston() && !piston->isExtending())
		{
			PistonHeadTile* pistonHead = (PistonHeadTile*)Tile::pistonHead;
			pistonHead->setHeadTexture(((PistonBaseTile*)tile)->getFaceTexture());
			m_tileRenderer.tesselateHeadPistonInWorldNoCulling(Tile::pistonHead, piston->m_pos, piston->getProgress(partialTicks) < 0.5f);
			pistonHead->resetHeadTexture();
			t.setOffset(pos.x - piston->m_pos.x, pos.y - piston->m_pos.y, pos.z - piston->m_pos.z);
			m_tileRenderer.tesselatePistonInWorldNoCulling(tile, piston->m_pos);
		}
		else
			m_tileRenderer.tesselateInWorldNoCulling(tile, piston->m_pos);

		t.setOffset(Vec3::ZERO);
		t.draw(RenderChunk::materialMap[TERRAIN_LAYER_BLEND]);
		Lighting::turnOn();
	}
}
