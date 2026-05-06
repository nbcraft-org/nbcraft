#pragma once

#include "API_D3D.hpp"
#include "renderer/hal/base/RenderContextBase.hpp"
#include "renderer/hal/enums/PrimitiveMode.hpp"

namespace mce
{
    class RenderContextD3D : public RenderContextBase
    {
    public:
        RenderContextD3D();

    protected:
        unsigned int _getPrimitiveCount(PrimitiveMode mode, unsigned int indexCount);
        unsigned int _getVertexCount(PrimitiveMode primitiveMode, unsigned int indexCount);
    };
}
