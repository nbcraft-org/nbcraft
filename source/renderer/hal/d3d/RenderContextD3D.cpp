#include "RenderContextD3D.hpp"
#include "common/Logger.hpp"

using namespace mce;

RenderContextD3D::RenderContextD3D()
    : RenderContextBase()
{
}

unsigned int RenderContextD3D::_getPrimitiveCount(PrimitiveMode mode, unsigned int indexCount)
{
    // D3D11 uses index count, but D3D needs the actual number of primitives.
    switch (mode)
    {
    case PRIMITIVE_MODE_QUAD_LIST:
    case PRIMITIVE_MODE_TRIANGLE_LIST:  return indexCount / 3;
    case PRIMITIVE_MODE_TRIANGLE_STRIP: return indexCount - 2;
    case PRIMITIVE_MODE_LINE_LIST:      return indexCount / 2;
    case PRIMITIVE_MODE_LINE_STRIP:     return indexCount - 1;
    default:
        LOG_E("Unknown PrimitiveMode: %d", mode);
        assert(false); // we don't want this shitting itself on release
        return 0;
    }
}
