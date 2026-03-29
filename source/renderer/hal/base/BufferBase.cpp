#include <utility>
#include <assert.h>
#include <cstring>
#include "BufferBase.hpp"
#include "compat/LegacyCPP.hpp"
#include "renderer/hal/interface/RenderContext.hpp"

using namespace mce;

BufferBase::BufferBase()
{
    m_clientData = nullptr;
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
    std::swap(this->m_clientData, other.m_clientData);
    std::swap(this->m_bufferType, other.m_bufferType);
    std::swap(this->m_stride, other.m_stride);
    std::swap(this->m_count, other.m_count);
    std::swap(this->m_internalSize, other.m_internalSize);
    std::swap(this->m_bufferOffset, other.m_bufferOffset);
}

void BufferBase::_createClientBuffer(RenderContext& context, const void* data, BufferType bufferType)
{
    assert(m_clientData == nullptr);
    m_clientData = new int8_t[m_internalSize];
    if (data)
        memcpy(m_clientData, data, m_internalSize);

    _bindClientBuffer(context);
}

void BufferBase::_bindClientBuffer(RenderContext& context)
{
    assert(m_clientData != nullptr);
    void*& activeBuffer = context.getActiveClientBuffer(m_bufferType);
    if (activeBuffer == m_clientData)
        return;

    activeBuffer = m_clientData;
}

void BufferBase::_resizeClientBuffer(RenderContext& context, const void* data, unsigned int size)
{
    if (m_clientData)
        delete[] m_clientData;

    int8_t* newBuffer = new int8_t[size];
    memcpy(newBuffer, data, size);

    m_clientData = newBuffer;
    m_internalSize = size;
}

void BufferBase::_updateClientBuffer(RenderContext& context, unsigned int stride, void*& data, unsigned int count)
{
    const unsigned int size = count * stride;

    if (size <= m_internalSize)
        memcpy(m_clientData, (int8_t*)data + m_bufferOffset, size);
    else
        _resizeClientBuffer(context, data, size);
}

void BufferBase::releaseBuffer()
{
    if (m_clientData)
    {
        delete[] m_clientData;
        m_clientData = nullptr;
    }

    m_stride = 0;
    m_bufferType = BUFFER_TYPE_NONE;
    m_count = 0;
}

void BufferBase::createBuffer(RenderContext& context, unsigned int stride, const void *data, unsigned int count, BufferType bufferType)
{
    m_stride = stride;
    m_count = count;
    m_internalSize = count * stride;
    m_bufferType = bufferType;
}

void BufferBase::createDynamicBuffer(RenderContext& context, unsigned int stride, const void* data, unsigned int count, BufferType bufferType)
{
    createBuffer(context, stride, data, count, bufferType);
}

void BufferBase::updateBuffer(RenderContext& context, unsigned int stride, void*& data, unsigned int count)
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
