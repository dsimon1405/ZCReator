#pragma once

#include <ZC/Tools/Container/ZC_DA.h>
#include "ZCR_Figure.h"

enum ZCR_FigureName
{
    ZCR_FN_Cube,
};

struct ZCR_FigureCreator
{
    static ZC_uptr<ZCR_Figure> CreateFigure(ZCR_FigureName figureName);

private:
    static std::string GetName(ZCR_FigureName standartFigure);
    static ZC_DA<ZC_Quad> GetQuads(ZCR_FigureName standartFigure);
    static ZC_DA<ZC_Triangle> GetTriangles(ZCR_FigureName standartFigure);
    static ZC_DA<int> GetNormals(ZCR_FigureName standartFigure);
};