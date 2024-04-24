#pragma once

#include "ZCR_Figure.h"

class ZCR_Figures
{
public:
    ZCR_Figures() = delete;

    static void CreateFigure(typename ZCR_Figure::Name figure);
    static void EraseFigure(ZCR_Figure* pFigure);
    static void SwitchFiguresAndActiveFiguresToNewSceneMode(ZCR_SceneModes mode);

private:
    static inline std::forward_list<ZC_uptr<ZCR_Figure>> figures;
};