#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
#include <ZC/Tools/Container/ZC_DA.h>
#include <ZC/Tools/Math/ZC_Figures.h>
#include <ZC/Tools/Math/ZC_Vec2.h>

struct ZCR_VBO
{
protected:
    ZC_Buffer vbo { GL_ARRAY_BUFFER };

    ZC_DA<ZC_Quad> quads;
    ZC_DA<ZC_Triangle> triangles;
    ZC_DA<ZC_Vec3<uchar>> colors;
    ZC_DA<int> normals;
    ZC_DA<ZC_Vec2<float>> texCoords;

    ZCR_VBO() = default;
    ZCR_VBO(ZC_DA<ZC_Quad>&& quads, ZC_DA<ZC_Triangle>&& triangles, ZC_DA<int>&& normals);

    enum StoredType
    {
        ST_Vertex,
        ST_Color,
        ST_Normal,
        ST_TexCoord,
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
    isVertexQuad - must be true if storing vertices of quads, and falls if triangle. In other cases no metter.
    */
    void BufferSubDataAllStoredType(StoredType storedType, bool isVertexQuad = true);

private:
    ulong trianglesStartIndex = 0;
    ulong colorsStartIndex = 0;
    ulong normatlsStartIndex = 0;
    ulong texCoordsStartIndex = 0;
    ulong texCoordsBSize = 0;
};