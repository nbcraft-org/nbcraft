#pragma once

#include <string>
#include "renderer/hal/enums/ShaderPrimitiveTypes.hpp"

namespace mce
{
    class ConstantBufferMetaData;

    class UniformMetaData
    {
    public:
        size_t m_numberOfElements;
        size_t m_byteOffset;
        ShaderPrimitiveTypes m_shaderPrimitiveType;
        ConstantBufferMetaData* m_constantBufferMetaDataParent;
        std::string m_uniformName;

    public:
        UniformMetaData();

        size_t getSize() const;
    };
}
