#include "TileEntityRenderDispatcher.hpp"
#include "TileEntityRenderer.hpp"
#include "MobSpawnerRenderer.hpp"
#include "SignRenderer.hpp"
#include "PistonRenderer.hpp"
#include "renderer/ShaderConstants.hpp"

Vec3 TileEntityRenderDispatcher::off;

TileEntityRenderDispatcher* TileEntityRenderDispatcher::instance;

TileEntityRenderDispatcher::TileEntityRenderDispatcher()
{
    m_pTextures = nullptr;
    m_pLevel = nullptr;
    m_pCamera = nullptr;
    m_pFont = nullptr;
    registerRenderer(TileEntityType::sign, new SignRenderer);
    registerRenderer(TileEntityType::mobSpawner, new MobSpawnerRenderer);
    registerRenderer(TileEntityType::piston, new PistonRenderer);
}

void TileEntityRenderDispatcher::registerRenderer(TileEntityType* type, TileEntityRendererBase* tileEntityRenderer)
{
    m_renderers[type] = tileEntityRenderer;
    tileEntityRenderer->init(this);
}

TileEntityRendererBase* TileEntityRenderDispatcher::getRenderer(const TileEntityType* type)
{
    if (!type) return nullptr;

    TileEntityRendererMap::iterator it = m_renderers.find(type);
    if (it != m_renderers.end())
        return it->second;

    return nullptr;
}

TileEntityRendererBase* TileEntityRenderDispatcher::getRenderer(TileEntity* e)
{
    return e ? getRenderer(e->getType()) : nullptr;
}

bool TileEntityRenderDispatcher::hasRenderer(TileEntity* e)
{
    return getRenderer(e) != nullptr;
}

void TileEntityRenderDispatcher::prepare(Level* level, Textures* textures, Font* font, const Mob* mob, float a)
{
    m_pLevel = level;
    m_pTextures = textures;
    m_pFont = font;
    m_pCamera = mob;

    m_rot = mob->m_oRot + (mob->m_rot - mob->m_oRot) * a;
    m_pos = mob->m_posPrev + (mob->m_pos - mob->m_posPrev) * a;
}

void TileEntityRenderDispatcher::render(TileEntity* e, float a)
{
    if (!e || !m_pLevel) return;

    if (e->distanceToSqr(m_pos) < 4096.0f)
    {
        float br = m_pLevel->getBrightness(e->m_pos);
        currentShaderColor = Color::WHITE;
        currentShaderDarkColor = Color(br, br, br);
        render(e, -off + e->m_pos, a);
    }
}

void TileEntityRenderDispatcher::render(TileEntity* e, const Vec3& vec, float a)
{
    TileEntityRendererBase* renderer = getRenderer(e);
    if (renderer)
        renderer->render(e, vec, a);
}

void TileEntityRenderDispatcher::setLevel(Level* level)
{
    m_pLevel = level;
}

float TileEntityRenderDispatcher::distanceToSqr(const Vec3& vec) const
{
    return vec.distanceToSqr(m_pos);
}

Font* TileEntityRenderDispatcher::getFont() const
{
    return m_pFont;
}

TileEntityRenderDispatcher* TileEntityRenderDispatcher::getInstance()
{
    if (!instance) instance = new TileEntityRenderDispatcher();
    return instance;
}
