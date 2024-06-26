#include "ZCR_VBO.h"

#include <ZC/Tools/Math/ZC_Vec2.h>

ZCR_VBO::ZCR_VBO(ZC_DA<ZC_Quad>&& _quads, ZC_DA<ZC_Triangle>&& _triangles, ZC_DA<int>&& _normals)
    : quads(std::move(_quads)),
    triangles(std::move(_triangles)),
    colors((quads.size * 4) + (triangles.size * 3)),
    normals(std::move(_normals))
{
    FillVBO();
}

void ZCR_VBO::FillVBO()
{
    trianglesStartIndex = quads.BytesSize();   //  quads byte size
    ulong trianglesBSize = triangles.BytesSize(),
        colorsBSize = colors.BytesSize(),
        normalsBSize = normals.BytesSize();
    texCoordsBSize = texCoords.BytesSize();
    colorsStartIndex = trianglesStartIndex + trianglesBSize;
    normatlsStartIndex = colorsStartIndex + colorsBSize;
    ulong texCoordsStartIndex = normatlsStartIndex + normalsBSize,
        vboSize = texCoordsStartIndex + texCoordsBSize;
    //  trianglesStartIndex is quads byte size
    vbo.GLNamedBufferData(vboSize, trianglesStartIndex != 0 ? quads.Begin() : nullptr, GL_DYNAMIC_DRAW);
    if (trianglesBSize != 0) vbo.GLNamedBufferSubData(trianglesStartIndex, trianglesBSize, triangles.Begin());
    // vbo.GLNamedBufferSubData(colorsStartIndex, colorsBSize, colors.Begin());
    vbo.GLNamedBufferSubData(normatlsStartIndex, normalsBSize, normals.Begin());
    if (texCoordsBSize != 0) vbo.GLNamedBufferSubData(texCoordsStartIndex, texCoordsBSize, texCoords.Begin());
}

void ZCR_VBO::BufferSubDataIndex(StoredType storedType, bool isQuad, ulong vertexIndex, const void* pData)
{
    static const ulong vertexByteSize = sizeof(ZC_Vec3<float>);
    static const ulong colorByteSize = sizeof(ZC_Vec3<uchar>);
    static const ulong normalByteSize = sizeof(int);
    static const ulong texCoordByteSize = sizeof(ZC_DA<ZC_Vec2<float>>);
    ulong offset = 0;
    ulong byteSize = 0;
    auto lambSetOffsetAndSize = [&offset, &byteSize, isQuad, vertexIndex](ulong startIndex, ulong typesByteSize, ulong trianglesStartIndex)
    {
        offset = isQuad ? startIndex + (vertexIndex * typesByteSize) : (startIndex + trianglesStartIndex + (vertexIndex * typesByteSize));
        byteSize = vertexByteSize;
    };

    switch (storedType)
    {
    case ST_Vertex: lambSetOffsetAndSize(0, vertexByteSize, trianglesStartIndex); break;
    case ST_Color: lambSetOffsetAndSize(colorsStartIndex, colorByteSize, trianglesStartIndex); break;
    case ST_Normal: lambSetOffsetAndSize(normatlsStartIndex, normalByteSize, trianglesStartIndex); break;
    case ST_TexCoord: lambSetOffsetAndSize(texCoordsStartIndex, texCoordByteSize, trianglesStartIndex); break;
    }
    vbo.GLNamedBufferSubData(offset, byteSize, pData);
}

void ZCR_VBO::BufferSubDataAllStoredType(StoredType storedType, bool isVertexQuad)
{
    switch (storedType)
    {
    case ST_Vertex: isVertexQuad ? vbo.GLNamedBufferSubData(0, trianglesStartIndex, quads.Begin()) :  //  quadsByteSize = trianglesStartIndex 
        vbo.GLNamedBufferSubData(trianglesStartIndex, colorsStartIndex - trianglesStartIndex, triangles.Begin()); break;   //  tianglesByteSize = colorsStartIndex - trianglesStartIndex
    case ST_Color: vbo.GLNamedBufferSubData(colorsStartIndex, normatlsStartIndex - colorsStartIndex, colors.Begin()); break;   //  colorsByteSize = texCoordsStartIndex - normatlsStartIndex
    case ST_Normal: vbo.GLNamedBufferSubData(normatlsStartIndex, texCoordsStartIndex - normatlsStartIndex, normals.Begin()); break; //  normalsByteSize = texCoordsStartIndex - normatlsStartIndex
    case ST_TexCoord: vbo.GLNamedBufferSubData(texCoordsStartIndex, texCoordsBSize, texCoords.Begin()); break;
    }
}