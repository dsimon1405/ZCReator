#include "ZCR_VBO.h"

#include <ZC/Tools/Math/ZC_Vec2.h>

ZCR_VBO::ZCR_VBO(ZC_DA<ZC_Quad>&& _quads, ZC_DA<ZC_Triangle>&& _triangles, ZC_DA<int>&& _normals)
    : quads(std::move(_quads)),
    triangles(std::move(_triangles)),
    colors((quads.size * 4) + (triangles.size * 3)),
    normals(std::move(_normals))
{}

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
    if (trianglesStartIndex != 0) vbo.BufferData(vboSize, &(quads.Begin()->bl), GL_DYNAMIC_DRAW);
    if (trianglesBSize != 0) vbo.BufferSubData(trianglesStartIndex, trianglesBSize, triangles.Begin());
    // vbo.BufferSubData(colorsStartIndex, colorsBSize, colors.Begin());
    vbo.BufferSubData(normatlsStartIndex, normalsBSize, normals.Begin());
    if (texCoordsBSize != 0) vbo.BufferSubData(texCoordsStartIndex, texCoordsBSize, texCoords.Begin());
}

void ZCR_VBO::BufferSubDataIndex(StoredType storedType, bool isQuad, ulong vertexIndex, const void* pData)
{
    static const ulong vertexByteSize = sizeof(ZC_Vec3<float>);
    static const ulong colorByteSize = sizeof(ZC_Vec3<uchar>);
    static const ulong normalByteSize = sizeof(int);
    static const ulong texCoordByteSize = sizeof(ZC_DA<ZC_Vec2<float>>);
    ulong offset = 0;
    ulong byteSize = 0;
    auto setOffsetAndSize = [&offset, &byteSize, isQuad, vertexIndex](ulong startIndex, ulong typesByteSize, ulong trianglesStartIndex)
    {
        offset = isQuad ? startIndex + (vertexIndex * typesByteSize) : (startIndex + trianglesStartIndex + (vertexIndex * typesByteSize));
        byteSize = vertexByteSize;
    };

    switch (storedType)
    {
    case ST_Vertex: setOffsetAndSize(0, vertexByteSize, trianglesStartIndex); break;
    case ST_Color: setOffsetAndSize(colorsStartIndex, colorByteSize, trianglesStartIndex); break;
    case ST_Normal: setOffsetAndSize(normatlsStartIndex, normalByteSize, trianglesStartIndex); break;
    case ST_TexCoord: setOffsetAndSize(texCoordsStartIndex, texCoordByteSize, trianglesStartIndex); break;
    }
    vbo.BufferSubData(offset, byteSize, pData);
}

void ZCR_VBO::BufferSubDataAllStoredType(StoredType storedType, bool isVertexQuad)
{
    switch (storedType)
    {
    case ST_Vertex: isVertexQuad ? vbo.BufferSubData(0, trianglesStartIndex, quads.Begin()) :  //  quadsByteSize = trianglesStartIndex 
        vbo.BufferSubData(trianglesStartIndex, colorsStartIndex - trianglesStartIndex, triangles.Begin()); break;   //  tianglesByteSize = colorsStartIndex - trianglesStartIndex
    case ST_Color: vbo.BufferSubData(colorsStartIndex, normatlsStartIndex - colorsStartIndex, colors.Begin()); break;   //  colorsByteSize = texCoordsStartIndex - normatlsStartIndex
    case ST_Normal: vbo.BufferSubData(normatlsStartIndex, texCoordsStartIndex - normatlsStartIndex, normals.Begin()); break; //  normalsByteSize = texCoordsStartIndex - normatlsStartIndex
    case ST_TexCoord: vbo.BufferSubData(texCoordsStartIndex, texCoordsBSize, texCoords.Begin()); break;
    }
}