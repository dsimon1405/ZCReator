#pragma once

#include <ZC/Tools/Container/ZC_DA.h>
#include "FigureSet/ZCR_FigureSet.h"

struct ZCR_Figure
{
    enum Name
    {
        Cube,
    };

    ZCR_Figure(Name standartFigure);
    // ZCR_Figure(ZCR_Figure&& fig);

    // virtual ~ZCR_Figure() = default;

    bool operator == (ZCR_Figure* fig) const noexcept;

    void Translate(const ZC_Vec3<float>& trans);

    void ChangeSceneMode(ZCR_SceneModes sceneMode);
    void ChangeSceneActivity(bool _isActiveOnScene);
    void ChangeSceneModeAndActivity(ZCR_SceneModes sceneMode, bool _isActiveOnScene);

private:
    std::string name;
    ZCR_FigureSet figureSet;

    static std::string GetName(Name standartFigure);
    static ZC_sptr<ZC_DA<ZC_Quad>> GetQuads(Name standartFigure);
    static ZC_sptr<ZC_DA<ZC_Triangle>> GetTriangles(Name standartFigure);
    static ZC_sptr<ZC_DA<int>> GetNormals(Name standartFigure);
};