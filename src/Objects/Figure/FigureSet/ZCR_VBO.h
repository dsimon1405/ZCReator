#pragma once

#include "ZCR_FigureData.h"

struct ZCR_VBO : public ZCR_FigureData
{
protected:
    ZC_sptr<ZC_Buffer> vbo = nullptr;
    
    ZCR_VBO() = default;
    ZCR_VBO(ZC_sptr<ZC_DA<ZC_Quad>>&& spQuads, ZC_sptr<ZC_DA<ZC_Triangle>>&& spTriangles, ZC_sptr<ZC_DA<int>>&& spNormals);

    enum StoredType
    {
        StoredType_Vertex,
        StoredType_Color,
        StoredType_Normal,
        StoredType_TexCoord,
    };
    
    void FillVBO();
    
    /*
    Storing data in vbo for index of concrete type.
    
    Params:
    storedType - stored type.
    isQuad - Points::SamePoint::isQuad.
    vertexIndex - Points::SamePoint::indexInQuadsOrTriangles.
    pData - pointer on data.
    isVertexQuad - must be true if storing vertices of quads, and falls if triangle. In other cases no metter.
    */
    void BufferSubDataIndex(StoredType storedType, bool isQuad, ulong vertexIndex, const void* pData);

    /*
    Storing data in vbo for one of types: vertex(quads or triangles), colors, normals, texCoords
    
    Params:
    storedType - stored type.
    pData - pointer on data.
    isVertexQuad - must be true if storing vertices of quads, and falls if triangle. In other cases no metter.
    */
    void BufferSubDataAllStoredType(StoredType storedType, const void* pData, bool isVertexQuad);

private:
    ulong trianglesStartIndex = 0;
    ulong colorsStartIndex = 0;
    ulong normatlsStartIndex = 0;
    ulong texCoordsStartIndex = 0;
    ulong texCoordsBSize = 0;
};