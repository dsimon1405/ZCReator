#pragma once

#include "ZCR_Figure.h"

struct ZCR_Cube : public ZCR_Figure
{
    ZCR_Cube();

    static ZC_DA<ZC_Quad> Quads();
    static ZC_DA<int> Normals();
};