#pragma once

#include <string.h>
#include <assert.h>

#include "compat/LegacyCPP.hpp"
#include "compat/LimitMacros.h"
#include "renderer/hal/enums/VertexField.hpp"
#include "VertexFieldFormat.hpp"

namespace mce
{
    class VertexFormat
    {
        struct FieldOffset
        {
            uint8_t m_offset;
            uint8_t m_size;
        };

    public:
        static const VertexFormat EMPTY;

    private:
        uint8_t m_fieldMask;
        uint8_t m_fieldOffset[5];
        uint8_t m_vertexSize;

    private:
        void _init()
		{
			m_fieldMask = 0;
			m_vertexSize = 0;
			memset(m_fieldOffset, UINT8_MAX, sizeof(m_fieldOffset));
		}

    public:
        VertexFormat() { _init(); }

    public:
        void enableField(VertexField vertexField)
		{
			if (hasField(vertexField)) return;

			m_fieldOffset[vertexField] = m_vertexSize;
			m_vertexSize += VertexFieldFormats::GetFormatByField(vertexField).size;
			if (m_vertexSize != 4 * (m_vertexSize >> 2))
				m_vertexSize  = 4 * (m_vertexSize >> 2) + 4;
			m_fieldMask |= (1 << vertexField);
		}

        bool hasField(VertexField vertexField) const
		{
			return HasField(m_fieldMask, vertexField);
		}

        const void* getFieldOffset(VertexField vertexField, const void *vertexData = nullptr) const
		{
			assert(m_fieldOffset[vertexField] != UINT8_MAX);
			return tryGetFieldOffset(vertexField, vertexData);
		}

        const void* tryGetFieldOffset(VertexField vertexField, const void *vertexData = nullptr) const
		{
			return (void*)((uintptr_t)vertexData + m_fieldOffset[vertexField]);
		}

        unsigned int getID() const { return m_fieldMask; }
        unsigned int getVertexSize() const { return m_vertexSize; }

        bool operator==(const VertexFormat &other) const
		{
			return m_fieldMask  == other.m_fieldMask
				&& m_vertexSize == other.m_vertexSize
				&& memcmp(m_fieldOffset, other.m_fieldOffset, sizeof(m_fieldOffset)) == 0;
		}

        bool operator!=(const VertexFormat &other) const
		{
			return m_fieldMask  != other.m_fieldMask
				|| m_vertexSize != other.m_vertexSize
				|| memcmp(m_fieldOffset, other.m_fieldOffset, sizeof(m_fieldOffset)) != 0;
		}

        bool operator<(const VertexFormat &other) const
		{
			return memcmp(this, &other, sizeof(VertexFormat)) < 0;
		}

        operator bool() const { return *this != VertexFormat::EMPTY; }

        static bool HasField(uint8_t fieldMask, VertexField vertexField)
		{
			return (fieldMask >> vertexField) & 1;
		}
    };
}
