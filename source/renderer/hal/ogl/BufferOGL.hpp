#pragma once

#include "API_OGL.hpp"
#include "renderer/hal/base/BufferBase.hpp"

namespace mce
{
    GLenum glTargetFromBufferType(BufferType bufferType);

    class BufferOGL : public BufferBase
    {
    protected:
        GLuint m_bufferName;
        GLenum m_target;
        GLenum m_usage;

    public:
        BufferOGL();
		MC_CTOR_MOVE(BufferOGL);
        ~BufferOGL();

    protected:
        void _createBuffer(RenderContext& context, ByteBuffer& data, BufferType bufferType);

    public:
		void _move(BufferOGL& other);
        void releaseBuffer();
        void bindBuffer(RenderContext& context);
		void createBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count, BufferType bufferType);
		void createDynamicBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count, BufferType bufferType);
        void resizeBuffer(RenderContext& context, ByteBuffer& data, unsigned int size);
        void updateBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count);
        bool isValid() const { return m_bufferName != GL_NONE || !m_clientBuffer.isEmpty(); }

        MC_FUNC_MOVE(BufferOGL);
	};
}
