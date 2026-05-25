#include <utility>
#include <assert.h>
#include <cstring>
#include "BufferBase.hpp"
#include "compat/LegacyCPP.hpp"
#include "renderer/hal/interface/RenderContext.hpp"

using namespace mce;

BufferBase::BufferBase()
{
    //m_clientData = nullptr;
    //m_bOwnsData = true;
    m_internalSize = 0;
    m_bufferOffset = 0;
    
    releaseBuffer();
}

BufferBase::~BufferBase()
{
    releaseBuffer();
}

void BufferBase::_init(BufferBase& other)
{
    // this might not be the right order to call this in, but this function is never called anyways
    _move(other);
    releaseBuffer();
}

void BufferBase::_move(BufferBase& other)
{
    this->m_clientBuffer = other.m_clientBuffer; // forces a swap
    std::swap(this->m_bufferType, other.m_bufferType);
    std::swap(this->m_stride, other.m_stride);
    std::swap(this->m_count, other.m_count);
    std::swap(this->m_internalSize, other.m_internalSize);
    std::swap(this->m_bufferOffset, other.m_bufferOffset);
}

void BufferBase::_createClientBuffer(RenderContext& context, ByteBuffer& data, BufferType bufferType)
{
    assert(m_clientBuffer.isEmpty());

    if (data)
    {
        m_clientBuffer.swap(data);
    }
    else
    {
        m_clientBuffer.reserve(m_internalSize);
    }

    _bindClientBuffer(context);
}

void BufferBase::_bindClientBuffer(RenderContext& context)
{
    assert(!m_clientBuffer.isEmpty());
    void*& activeBuffer = context.getActiveClientBuffer(m_bufferType);
    if (activeBuffer == m_clientBuffer.getData())
        return;

    activeBuffer = (void*)m_clientBuffer.getData();
}

void BufferBase::_resizeClientBuffer(RenderContext& context, ByteBuffer& data, unsigned int size)
{
    if (data)
    {
        m_clientBuffer.swap(data);
    }
    else
    {
        //memcpy(newBuffer, data, size);
        m_clientBuffer.reserve(m_internalSize);
    }

    m_internalSize = size;
}

void BufferBase::_updateClientBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count)
{
    const unsigned int size = count * stride;

    assert(m_bufferOffset == 0);
    assert(!data.isEmpty());
    
    m_clientBuffer.assign(data);
    
    m_internalSize = size;

    /*if (size <= m_internalSize)
    {
        memcpy(m_clientData, (int8_t*)data + m_bufferOffset, size);
    }
    else
    {
        _resizeClientBuffer(context, data, size);
    }*/
}

void BufferBase::releaseBuffer()
{
    m_clientBuffer.clear();
    m_stride = 0;
    m_bufferType = BUFFER_TYPE_NONE;
    m_count = 0;
}

void BufferBase::createBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count, BufferType bufferType)
{
    m_stride = stride;
    m_count = count;
    m_internalSize = count * stride;
    m_bufferType = bufferType;
}

void BufferBase::createDynamicBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count, BufferType bufferType)
{
    createBuffer(context, stride, data, count, bufferType);
}

void BufferBase::updateBuffer(RenderContext& context, unsigned int stride, ByteBuffer& data, unsigned int count)
{
    m_stride = stride;
    m_count = count;
}

void BufferBase::copy(BufferBase& other)
{
    // make sure this works, this function is never called
    other.m_bufferType = this->m_bufferType;
    other.m_stride = this->m_stride;
    other.m_count = this->m_count;
    other.m_internalSize = this->m_internalSize;
}
