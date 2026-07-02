#pragma once

#include <string>
#include "TileEntityRenderDispatcher.hpp"
#include "client/renderer/Textures.hpp"
#include "client/renderer/Font.hpp"

class TileEntityRendererBase
{
public:
    virtual ~TileEntityRendererBase()
    {
    }

    virtual void init(TileEntityRenderDispatcher* dispatcher) = 0;
    virtual void render(TileEntity* entity, const Vec3& pos, float partialTicks) = 0;
};

template<typename T>
class TileEntityRenderer : public TileEntityRendererBase
{
public:
    TileEntityRenderer()
        : m_pDispatcher(nullptr)
    {
    }

    void init(TileEntityRenderDispatcher* dispatcher) override 
    {
        m_pDispatcher = dispatcher;
    }

    void render(TileEntity* entity, const Vec3& pos, float partialTicks) override 
    {
        renderTyped(static_cast<T*>(entity), pos, partialTicks);
    }

    virtual void renderTyped(T* entity, const Vec3& pos, float partialTicks) = 0;

protected:
    TileEntityRenderDispatcher* m_pDispatcher;

    void bindTexture(const std::string& resourceName)
    {
        if (!m_pDispatcher || !m_pDispatcher->m_pTextures) return;
        m_pDispatcher->m_pTextures->loadAndBindTexture(resourceName);
    }

    //void bindTexture(const std::string& urlTexture, const std::string& fallbackTexture)
    //{
    //    if (!tileEntityRenderDispatcher || !tileEntityRenderDispatcher->m_pTextures) return;
    //    Textures* t = tileEntityRenderDispatcher->m_pTextures;
    //    t->bind(t->loadHttpTexture(urlTexture, fallbackTexture));
    //}

    Font* getFont()
    {
        return m_pDispatcher ? m_pDispatcher->getFont() : nullptr;
    }

    Level* getLevel()
    {
        return m_pDispatcher ? m_pDispatcher->m_pLevel : nullptr;
    }
};
