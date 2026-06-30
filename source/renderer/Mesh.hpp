#pragma once

#include "renderer/hal/enums/PrimitiveMode.hpp"
#include "renderer/hal/interface/Buffer.hpp"
#include "renderer/hal/interface/VertexBufferState.hpp"

#include "VertexFormat.hpp"
#include "MaterialPtr.hpp"

namespace mce
{
    class Mesh
    {
    public:
        unsigned int m_indexCount;
        unsigned int m_vertexCount;
        PrimitiveMode m_primitiveMode;
        VertexFormat m_vertexFormat;
        VertexBufferState m_vertexBufferState;
        uint8_t m_indexSize;
        Buffer m_vertexBuffer;
        Buffer m_indexBuffer;
        ByteBuffer* m_pRawData;

    public:
        Mesh();
        MC_CTOR_MOVE(Mesh);
        Mesh(const VertexFormat& vertexFormat, unsigned int vertexCount, unsigned int indexCount, uint8_t indexSize, PrimitiveMode primitiveMode, ByteBuffer& data, bool temporary);
        ~Mesh();

    protected:
        void _move(Mesh& other);

    public:
        void reset();
        bool loadRawData(RenderContext& context, ByteBuffer& data);
        void render(const MaterialPtr& materialPtr, unsigned int startOffset = 0, unsigned int count = 0);
        bool isValid() const;
        bool isTemporary() const;

    public:
        MC_FUNC_MOVE(Mesh);

    public:
        static void clearGlobalBuffers();
    };
}
