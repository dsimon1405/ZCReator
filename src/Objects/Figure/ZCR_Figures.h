#pragma once

#include "ZCR_FigureCreator.h"

class ZCR_Figures
{
public:
    ZCR_Figures();
    ~ZCR_Figures();

    static void CreateFigure(ZCR_FigureName figureName);
    static void EraseFigure(ZCR_Figure* pFigure);
    static void SwitchFiguresAndActiveFiguresToNewSceneMode(ZCR_SceneMode mode);

private:
    static inline ZCR_Figures* pFigures;

    std::forward_list<ZC_uptr<ZCR_Figure>> figures;
};