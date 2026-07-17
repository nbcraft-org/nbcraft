#pragma once

#include "world/level/Level.hpp"

#include "client/renderer/Font.hpp"
#include "client/renderer/Textures.hpp"
#include "world/tile/entity/TileEntity.hpp"
#include "world/entity/Player.hpp"
#include "client/renderer/renderer/EntityShaderManager.hpp"

class TileEntityRendererBase;

class TileEntityRenderDispatcher : public EntityShaderManager
{
private:
    TileEntityRenderDispatcher();

public:
    typedef HashMap<const TileEntityType*, TileEntityRendererBase*> TileEntityRendererMap;

    void registerRenderer(TileEntityType*, TileEntityRendererBase*);

    TileEntityRendererBase* getRenderer(const TileEntityType*);

    TileEntityRendererBase* getRenderer(TileEntity* e);

    bool hasRenderer(TileEntity* e);

    void prepare(Level* level, Textures* textures, Font* font, const Mob* camera, float a);

    void render(TileEntity* e, float a);
    void render(TileEntity* e, const Vec3&, float a);

    void setLevel(Level* level);
    float distanceToSqr(const Vec3&) const;
    Font* getFont() const;
    static TileEntityRenderDispatcher* getInstance();

private:
    TileEntityRendererMap m_renderers;
    Font* m_pFont;
    static TileEntityRenderDispatcher* instance;

public:
    Textures* m_pTextures;
    Level* m_pLevel;
    const Mob* m_pCamera;
    Rot2 m_rot;
    Vec3 m_pos;
    static Vec3 off;
};
