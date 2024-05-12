#pragma once

#include "ZCR_VBO.h"
#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>

struct ZCR_Surface : public virtual ZCR_VBO
{
    ZCR_Surface();
    ZCR_Surface(ZCR_Surface&& s);

    ZC_DrawerSet dsSurface;
    ZC_DSController dsConSurface;

    ZC_DrawerSet CreateSurfaceDrawerSet();
    ZC_DSController MakeSurfaceRSController();
    ZC_DA<uchar> GetTriangleElements(ulong& rElementsCount, GLenum& rElementsType);
    template<typename T>
    static void FillTriangleElements(T* pElementsHead, ulong elementsSize, ulong quadsElementsCount);

    void SwitchRSControllerTriangle(ZC_DrawerLevel drawerLevel);
};

template<typename T>
void ZCR_Surface::FillTriangleElements(T* pElementsHead, ulong elementsSize, ulong quadsElementsCount)
{
    ulong elementsI = 0,
        index = 0;
    for ( ; elementsI < quadsElementsCount; elementsI += 6)
    {
        pElementsHead[elementsI] = pElementsHead[elementsI + 3] = static_cast<T>(index++);
        pElementsHead[elementsI + 1] = pElementsHead[elementsI + 5] = static_cast<T>(index++);
        pElementsHead[elementsI + 2] = static_cast<T>(index++);
        pElementsHead[elementsI + 4] = static_cast<T>(index++);
    }
    while (elementsI < elementsSize)
    {
        pElementsHead[elementsI++] = static_cast<T>(index++);
        pElementsHead[elementsI++] = static_cast<T>(index++);
        pElementsHead[elementsI++] = static_cast<T>(index++);
    }
}