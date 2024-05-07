#pragma once

#include <ZC/Tools/Container/ZC_DA.h>
#include "FigureSet/ZCR_FigureSet.h"

enum FigureName
{
    FN_Cube,
};

struct ZCR_Figure
{
    ZCR_Figure(FigureName standartFigure);
    // ZCR_Figure(ZCR_Figure&& fig);

    // virtual ~ZCR_Figure() = default;

    bool operator == (ZCR_Figure* fig) const noexcept;

    void Translate(const ZC_Vec3<float>& trans);

    void ChangeSceneMode(ZCR_SceneMode sceneMode);
    void ChangeSceneActivity(bool _isActiveOnScene);
    void ChangeSceneModeAndActivity(ZCR_SceneMode sceneMode, bool _isActiveOnScene);

private:
    std::string name;
    ZCR_FigureSet figureSet;

    static std::string GetName(FigureName standartFigure);
    static ZC_sptr<ZC_DA<ZC_Quad>> GetQuads(FigureName standartFigure);
    static ZC_sptr<ZC_DA<ZC_Triangle>> GetTriangles(FigureName standartFigure);
    static ZC_sptr<ZC_DA<int>> GetNormals(FigureName standartFigure);
};