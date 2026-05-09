#include "VertexBufferStateBase.hpp"

using namespace mce;

VertexBufferStateBase::VertexBufferStateBase()
{
}

void VertexBufferStateBase::createVertexBufferState(RenderContext& context, const VertexBufferStateDescription& desc)
{
    m_description = desc;
}

bool VertexBufferStateBase::bindVertexBufferState(RenderContext& context)
{
    return context.m_currentState.m_vertexBufferStateDescription == m_description;
}
