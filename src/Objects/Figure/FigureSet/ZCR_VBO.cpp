#include "ZCR_VBO.h"

#include <ZC/Tools/Math/ZC_Vec2.h>

ZCR_VBO::ZCR_VBO(ZC_sptr<ZC_DA<ZC_Quad>>&& _spQuads, ZC_sptr<ZC_DA<ZC_Triangle>>&& _spTriangles, ZC_sptr<ZC_DA<int>>&& _spNormals)
    : ZCR_FigureData(std::move(_spQuads), std::move(_spTriangles), std::move(_spNormals)),
    vbo(ZC_sptrMake<ZC_Buffer>(GL_ARRAY_BUFFER))
{}

void ZCR_VBO::FillVBO()
{
    trianglesStartIndex = spQuads ? spQuads->BytesSize() : 0;   //  quads byte size
    ulong trianglesBSize = spTriangles ? spTriangles->BytesSize() : 0,
        colorsBSize = colors.BytesSize(),
        normalsBSize = spNormals->BytesSize();
    texCoordsBSize = spTexCoords ? spTexCoords->BytesSize() : 0;
    colorsStartIndex = trianglesStartIndex + trianglesBSize;
    normatlsStartIndex = colorsStartIndex + colorsBSize;
    ulong texCoordsStartIndex = normatlsStartIndex + normalsBSize,
        vboSize = texCoordsStartIndex + texCoordsBSize;
    if (trianglesStartIndex != 0) vbo->BufferData(vboSize, &(spQuads->Begin()->bl), GL_DYNAMIC_DRAW);
    if (trianglesBSize != 0) vbo->BufferSubData(trianglesStartIndex, trianglesBSize, spTriangles->Begin());
    // vbo->BufferSubData(colorsStartIndex, colorsBSize, colors.Begin());
    vbo->BufferSubData(normatlsStartIndex, normalsBSize, spNormals->Begin());
    if (texCoordsBSize != 0) vbo->BufferSubData(texCoordsStartIndex, texCoordsBSize, spTexCoords->Begin());
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
    case StoredType_Vertex: setOffsetAndSize(0, vertexByteSize, trianglesStartIndex); break;
    case StoredType_Color: setOffsetAndSize(colorsStartIndex, colorByteSize, trianglesStartIndex); break;
    case StoredType_Normal: setOffsetAndSize(normatlsStartIndex, normalByteSize, trianglesStartIndex); break;
    case StoredType_TexCoord: setOffsetAndSize(texCoordsStartIndex, texCoordByteSize, trianglesStartIndex); break;
    }
    vbo->BufferSubData(offset, byteSize, pData);
}

void ZCR_VBO::BufferSubDataAllStoredType(StoredType storedType, const void* pData, bool isVertexQuad)
{
    switch (storedType)
    {
    case StoredType_Vertex: isVertexQuad ? vbo->BufferSubData(0, trianglesStartIndex, pData) :  //  quadsByteSize = trianglesStartIndex 
        vbo->BufferSubData(trianglesStartIndex, colorsStartIndex - trianglesStartIndex, pData); break;   //  tianglesByteSize = colorsStartIndex - trianglesStartIndex
    case StoredType_Color: vbo->BufferSubData(colorsStartIndex, normatlsStartIndex - colorsStartIndex, pData); break;   //  colorsByteSize = texCoordsStartIndex - normatlsStartIndex
    case StoredType_Normal: vbo->BufferSubData(normatlsStartIndex, texCoordsStartIndex - normatlsStartIndex, pData); break; //  normalsByteSize = texCoordsStartIndex - normatlsStartIndex
    case StoredType_TexCoord: vbo->BufferSubData(texCoordsStartIndex, texCoordsBSize, pData); break;
    }
}