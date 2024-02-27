#include "ZCR_FigureData.h"

ZCR_FigureData::ZCR_FigureData(ZC_sptr<ZC_DA<ZC_Quad>>&& _spQuads, ZC_sptr<ZC_DA<ZC_Triangle>>&& _spTriangles, ZC_sptr<ZC_DA<int>>&& _spNormals)
    : spQuads(std::move(_spQuads)),
    spTriangles(std::move(_spTriangles)),
    colors((spQuads ? (spQuads->size * 4) : 0) + (spTriangles ? (spTriangles->size * 3) : 0)),
    spNormals(std::move(_spNormals))
{}