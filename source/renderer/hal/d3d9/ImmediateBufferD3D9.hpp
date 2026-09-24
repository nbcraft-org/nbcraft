#pragma once

#include "API_D3D9.hpp"
#include "renderer/hal/base/ImmediateBufferBase.hpp"
#include "BufferD3D9.hpp"

namespace mce
{
    class ImmediateBufferD3D9 : public ImmediateBufferBase, BufferD3D9
    {
    public:
        ImmediateBufferD3D9();

    public:
        void createDynamicBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count, BufferType bufferType);
        void updateBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count);

        bool isValid() const;
    };
}
