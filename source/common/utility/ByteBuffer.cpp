#include <algorithm>
#include <cstring>
#include "ByteBuffer.hpp"

const ByteBuffer ByteBuffer::EMPTY;

ByteBuffer::ByteBuffer()
{
    m_data = nullptr;
    m_size = 0;
    m_bIsOrphaned = false;
    m_bIsReference = false;
}

ByteBuffer::~ByteBuffer()
{
    clear();
}

void ByteBuffer::_init(ByteBuffer& other)
{
    _move(other);
    clear();
}

void ByteBuffer::_move(ByteBuffer& other)
{
    std::swap(this->m_data, other.m_data);
    std::swap(this->m_size, other.m_size);
}

void ByteBuffer::assign(ByteBuffer& other)
{
    if (other.isOrphaned())
    {
        swap(other);
        return;
    }
    
    m_data = other.m_data;
    m_size = other.m_size;
    m_bIsReference = true;
}

void ByteBuffer::clear()
{
    if (m_data && !m_bIsReference)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    m_size = 0;
    m_bIsOrphaned = false;
    m_bIsReference = false;
}

bool ByteBuffer::resize(size_t newSize)
{
    if (m_size >= newSize)
        return false;

    /*if (m_size == newSize)
        return false;*/

    uint8_t* newData = new uint8_t[newSize];

    if (m_data)
    {
        memcpy(newData, m_data, m_size);
        delete[] m_data;
    }

    m_data = newData;
    m_size = newSize;

    return true;
}

/*void ByteBuffer::reserve(size_t size)
{
    if (m_size >= size)
        return;

    resize(size);
}*/
