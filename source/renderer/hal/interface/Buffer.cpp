#include <utility>

#include "Buffer.hpp"
#include "RenderContext.hpp"

using namespace mce;

void Buffer::createIndexBuffer(RenderContext& context, unsigned int sizeOfSingleIndice, ByteBuffer& indices, unsigned int numberOfIndices)
{
    createBuffer(context, sizeOfSingleIndice, indices, numberOfIndices, BUFFER_TYPE_INDEX);
}

void Buffer::createVertexBuffer(RenderContext& context, unsigned int vertexStride, ByteBuffer& vertices, unsigned int numberOfVertices)
{
    createBuffer(context, vertexStride, vertices, numberOfVertices, BUFFER_TYPE_VERTEX);
}

void Buffer::createDynamicIndexBuffer(RenderContext& context, unsigned int stride, unsigned int count)
{
    ByteBuffer nothing;
    createDynamicBuffer(context, stride, nothing, count, BUFFER_TYPE_INDEX);
}

void Buffer::createDynamicVertexBuffer(RenderContext& context, unsigned int stride, unsigned int count)
{
    ByteBuffer nothing;
    createDynamicBuffer(context, stride, nothing, count, BUFFER_TYPE_VERTEX);
}
