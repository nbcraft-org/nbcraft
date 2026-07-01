#include "VertexBufferStateOGL.hpp"
#include "helpers/ErrorHandlerOGL.hpp"

#ifndef FEATURE_GFX_SHADERS

using namespace mce;

VertexBufferStateOGL::VertexBufferStateOGL()
{
}

void VertexBufferStateOGL::createVertexBufferState(RenderContext& context, const VertexBufferStateDescription& desc)
{
    VertexBufferStateBase::createVertexBufferState(context, desc);

    if (!context.m_currentState.m_bBoundVertexBufferState)
    {
        bindVertexBufferState(context, true);
        context.m_currentState.m_bBoundVertexBufferState = true;
        context.m_currentState.m_vertexBufferStateDescription = desc;
    }
}

bool VertexBufferStateOGL::bindVertexBufferState(RenderContext& context, bool forceBind)
{
    VertexBufferStateDescription& ctxDesc = context.m_currentState.m_vertexBufferStateDescription;

    VertexFormat& ctxVertexFormat = ctxDesc.vertexFormat;
    VertexFormat& vertexFormat = m_description.vertexFormat;

    void* vertexData = context.getActiveClientBuffer(BUFFER_TYPE_VERTEX);
    unsigned int vertexSize = vertexFormat.getVertexSize();

    if (forceBind || vertexFormat.hasField(VERTEX_FIELD_POSITION) != ctxVertexFormat.hasField(VERTEX_FIELD_POSITION))
    {
        if (vertexFormat.hasField(VERTEX_FIELD_POSITION))
        {
            xglEnableClientState(GL_VERTEX_ARRAY);
        }
        else
        {
            xglDisableClientState(GL_VERTEX_ARRAY);
        }

        ErrorHandlerOGL::checkForErrors();
    }

    if (forceBind || vertexFormat.hasField(VERTEX_FIELD_UV0) != ctxVertexFormat.hasField(VERTEX_FIELD_UV0))
    {
        if (vertexFormat.hasField(VERTEX_FIELD_UV0))
        {
            xglEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        else
        {
            xglDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }

        ErrorHandlerOGL::checkForErrors();
    }

    if (forceBind || vertexFormat.hasField(VERTEX_FIELD_COLOR) != ctxVertexFormat.hasField(VERTEX_FIELD_COLOR))
    {
        if (vertexFormat.hasField(VERTEX_FIELD_COLOR))
        {
            xglEnableClientState(GL_COLOR_ARRAY);
        }
        else
        {
            xglDisableClientState(GL_COLOR_ARRAY);
        }

        ErrorHandlerOGL::checkForErrors();
    }

#ifdef USE_GL_NORMAL_LIGHTING
    if (forceBind || vertexFormat.hasField(VERTEX_FIELD_NORMAL) != ctxVertexFormat.hasField(VERTEX_FIELD_NORMAL))
    {
        if (vertexFormat.hasField(VERTEX_FIELD_NORMAL))
        {
            xglEnableClientState(GL_NORMAL_ARRAY);
        }
        else
        {
            xglDisableClientState(GL_NORMAL_ARRAY);
        }

        ErrorHandlerOGL::checkForErrors();
    }
#endif

    if (vertexFormat.hasField(VERTEX_FIELD_POSITION))
    {
        const RenderContextOGL::VertexFieldFormat& field = RenderContextOGL::vertexFieldFormats[VERTEX_FIELD_POSITION];
        xglVertexPointer(field.components, field.componentsType, vertexSize, vertexFormat.getFieldOffset(VERTEX_FIELD_POSITION, vertexData));
        ErrorHandlerOGL::checkForErrors();
    }

    if (vertexFormat.hasField(VERTEX_FIELD_UV0))
    {
        const RenderContextOGL::VertexFieldFormat& field = RenderContextOGL::vertexFieldFormats[VERTEX_FIELD_UV0];
        xglTexCoordPointer(field.components, field.componentsType, vertexSize, vertexFormat.getFieldOffset(VERTEX_FIELD_UV0, vertexData));
        ErrorHandlerOGL::checkForErrors();
    }

    if (vertexFormat.hasField(VERTEX_FIELD_COLOR))
    {
        const RenderContextOGL::VertexFieldFormat& field = RenderContextOGL::vertexFieldFormats[VERTEX_FIELD_COLOR];
        xglColorPointer(field.components, field.componentsType, vertexSize, vertexFormat.getFieldOffset(VERTEX_FIELD_COLOR, vertexData));
        ErrorHandlerOGL::checkForErrors();
    }

    if (vertexFormat.hasField(VERTEX_FIELD_NORMAL))
    {
        const RenderContextOGL::VertexFieldFormat& field = RenderContextOGL::vertexFieldFormats[VERTEX_FIELD_NORMAL];
        xglNormalPointer(field.componentsType, vertexSize, vertexFormat.getFieldOffset(VERTEX_FIELD_NORMAL, vertexData));
        ErrorHandlerOGL::checkForErrors();
    }

    ctxVertexFormat = vertexFormat;

    return VertexBufferStateBase::bindVertexBufferState(context);
}

#endif // !defined(FEATURE_GFX_SHADERS)
