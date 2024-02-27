#pragma once

#include "ZCR_Figure.h"

struct ZCR_ActiveFigures
{
    static void Add(ZCR_Figure* pFigure);
    static void Erase(ZCR_Figure* pFigure);
    static void Clear(bool changeActivityOnScene);
    static bool IsEmpty();

    static inline std::forward_list<ZCR_Figure*> activeFigures;
};