#pragma once

#include "renderer/hal/VertexBufferStateDescription.hpp"
#include "renderer/hal/interface/RenderContext.hpp"

namespace mce
{
    class VertexBufferStateBase
    {
    public:
        VertexBufferStateDescription m_description;

    public:
        VertexBufferStateBase();

    public:
        void createVertexBufferState(RenderContext& context, const VertexBufferStateDescription& desc);
        bool bindVertexBufferState(RenderContext& context);
    };
}
