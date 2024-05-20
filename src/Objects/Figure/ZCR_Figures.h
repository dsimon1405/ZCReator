#pragma once

#include "ZCR_FigureCreator.h"
#include "ImGui/ZCR_IGW_Figures.h"

class ZCR_Figures
{
public:
    ZCR_Figures();
    ~ZCR_Figures();

    static void CreateFigure(ZCR_FigureName figureName);
    static void EraseFigure(ZCR_Figure* pFigure);
    static void SwitchSceneMode(ZCR_SceneMode mode);
    static ZCR_Figure* GetActiveFigure();
    static void MakeFigureActive(ZCR_Figure* pFigure);

private:
    static inline ZCR_Figures* pFigures;

    ZCR_Figure* pActiveFigure = nullptr;

    std::list<ZC_uptr<ZCR_Figure>> figures;
    ZCR_IGW_Figures igwFigures { figures };
};