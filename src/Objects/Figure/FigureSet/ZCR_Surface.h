#pragma once

#include "ZCR_VBO.h"
#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>

struct ZCR_Surface : public virtual ZCR_VBO
{
    ZCR_Surface();

    ZC_sptr<ZC_RendererSet> spRendererSetsSucafe;
    ZC_sptr<ZC_RendererSetAndDrawingSet> spRSADSSurface;

    ZC_sptr<ZC_RendererSet> MakeTriangleRendererSet();
    ZC_DA<uchar> GetTriangleElements(size_t& rElementsCount, GLenum& rElementsType);
    template<typename TpHead>
    static void FillTriangleElements(TpHead pElementsHead, size_t elementsSize, size_t quadsElementsCount);

    typedef typename ZC_RendererSet::Level RSLvl;
    void SwitchRSandDSTriangle(RSLvl lvl);
};

template<typename TpHead>
void ZCR_Surface::FillTriangleElements(TpHead pElementsHead, size_t elementsSize, size_t quadsElementsCount)
{
    size_t elementsI = 0,
        index = 0;
    for ( ; elementsI < quadsElementsCount; elementsI += 6)
    {
        pElementsHead[elementsI] = pElementsHead[elementsI + 3] = index++;
        pElementsHead[elementsI + 1] = pElementsHead[elementsI + 5] = index++;
        pElementsHead[elementsI + 2] = index++;
        pElementsHead[elementsI + 4] = index++;
    }
    while (elementsI < elementsSize)
    {
        pElementsHead[elementsI++] = index++;
        pElementsHead[elementsI++] = index++;
        pElementsHead[elementsI++] = index++;
    }
}