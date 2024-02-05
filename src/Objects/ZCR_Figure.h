#pragma once

#include <ZC/Tools/Container/ZC_DynamicArray.h>
#include "ZCR_Point.h"
#include "ZCR_Triangle.h"
#include "ZCR_Line.h"

struct ZCR_Figure
{
    ZC_DA<unsigned char> colors;
    ZCR_Triangle triangles;
    ZCR_Point points;
    ZCR_Line lines;

    ZCR_Figure(ZC_DA<ZC_Quad>&& quads, ZC_DA<unsigned char>&& _colors, ZC_DA<int>&& normals, const ZC_Mat4<float>& model);

    ZCR_Figure(ZCR_Figure&& fig);

    static ZC_DA<unsigned char> BlackColors(size_t count);
};