#pragma once

#include "API_OGL.hpp"

#ifndef FEATURE_GFX_SHADERS

#include "renderer/hal/base/VertexBufferStateBase.hpp"

namespace mce
{
    class VertexBufferStateOGL : public VertexBufferStateBase
    {
    public:
        VertexBufferStateOGL();

    public:
        void createVertexBufferState(RenderContext& context, const VertexBufferStateDescription& desc);
        bool bindVertexBufferState(RenderContext& context, bool forceBind = false);
    };
}

#endif // !defined(FEATURE_GFX_SHADERS)
