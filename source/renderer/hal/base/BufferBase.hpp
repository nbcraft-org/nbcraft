#pragma once

#include <stdint.h>

#include "compat/LegacyCPP.hpp"
#include "common/utility/ByteBuffer.hpp"
#include "renderer/hal/enums/BufferType.hpp"

namespace mce
{
    class RenderContext;
	class BufferBase
	{
	public:
        ByteBuffer m_clientBuffer;
        BufferType m_bufferType;
        unsigned int m_stride;
        unsigned int m_count;
        unsigned int m_internalSize;
        unsigned int m_bufferOffset;
		
	protected:
		void _init(BufferBase& other);
		void _move(BufferBase& other);
        void _createClientBuffer(RenderContext& context, ByteBuffer& data, BufferType bufferType);
        void _bindClientBuffer(RenderContext& context);
        void _resizeClientBuffer(RenderContext& context, ByteBuffer& data, unsigned int size);
        void _updateClientBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count);
        bool _isClientBuffer() const { return !m_clientBuffer.isEmpty(); }

	public:
        BufferBase();
        ~BufferBase();
        MC_CTOR_MOVE_CUSTOM(BufferBase);

        void releaseBuffer();
        void bindBuffer(RenderContext& context) {}
		void createBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count, BufferType bufferType);
		void createDynamicBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count, BufferType bufferType);
        void resizeBuffer(RenderContext& context, ByteBuffer& data, unsigned int size) {}
        void updateBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count);
		void copy(BufferBase& other);
        unsigned int getInternalBufferSize() const { return m_internalSize; }
        bool isValid() const { return m_internalSize > 0; }

        MC_FUNC_MOVE(BufferBase);
	};
}
