#pragma once

#include <string>
#include "renderer/hal/enums/ShaderPrimitiveTypes.hpp"

namespace mce
{
    class UniformMetaData;

    class ShaderConstantBase
    {
    public:
        std::string m_name;
        size_t m_numberOfElements;
        size_t m_byteOffset;
        ShaderPrimitiveTypes m_shaderPrimitiveType;
        bool m_dirty;

    private:
        void _init();

    public:
        ShaderConstantBase() { _init(); }
        ShaderConstantBase(const UniformMetaData&);
        virtual ~ShaderConstantBase();

        void release();
        const std::string& getName() const { return m_name; }
        size_t getSize() const { return ShaderPrimitiveTypeHelper::sizeInBytesFromShaderPrimitiveType(m_shaderPrimitiveType); }
        ShaderPrimitiveTypes getType() const { return m_shaderPrimitiveType; }
        bool isDirty() const { return m_dirty; }

        bool operator==(const ShaderConstantBase& other) const;
    };
}
