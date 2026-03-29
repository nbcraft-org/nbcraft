#include <cstring>
#include "RenderContextBase.hpp"

using namespace mce;

RenderContextBase::RenderContextBase()
{
    m_pRenderDevice = nullptr;
    _clearArrays();
}

void RenderContextBase::_clearArrays()
{
    memset(m_lastShaderPrograms, 0, sizeof(ShaderProgram*) * SHADER_TYPES_COUNT);
    memset(m_activeClientBuffers, 0, sizeof(void*) * BUFFER_TYPES_COUNT);
}

bool RenderContextBase::setShadeMode(ShadeMode mode)
{
    if (m_currentState.m_bBoundShadeMode && m_currentState.m_shadeMode == mode)
        return false;

    m_currentState.m_shadeMode = mode;
    m_currentState.m_bBoundShadeMode = true;

    return true;
}

bool RenderContextBase::setCurrentColor(const Color& color)
{
    if (m_currentState.m_bBoundColor && m_currentState.m_color == color)
        return false;

    m_currentState.m_color = color;
    m_currentState.m_bBoundColor = true;

    return true;
}

bool RenderContextBase::setGamma(Gamma gamma)
{
    if (m_currentState.m_bBoundGamma && m_currentState.m_gamma == gamma)
        return false;

    m_currentState.m_gamma = gamma;
    m_currentState.m_bBoundGamma = true;

    return true;
}

void RenderContextBase::clearContextState()
{
	m_currentState.clear();
    m_immediateBuffer = ImmediateBuffer();

    _clearArrays();
}

void RenderContextBase::lostContext()
{
    clearContextState();
}

void*& RenderContextBase::getActiveClientBuffer(BufferType bufferType)
{
#ifdef _DEBUG
    if (bufferType < BUFFER_TYPES_MIN || bufferType > BUFFER_TYPES_MAX)
        throw std::out_of_range("m_activeClientBuffers[]");
#endif

    return m_activeClientBuffers[bufferType];
}

const void* RenderContextBase::getActiveClientBuffer(BufferType bufferType) const
{
#ifdef _DEBUG
    if (bufferType < BUFFER_TYPES_MIN || bufferType > BUFFER_TYPES_MAX)
        throw std::out_of_range("m_activeClientBuffers[]");
#endif

    return m_activeClientBuffers[bufferType];
}
