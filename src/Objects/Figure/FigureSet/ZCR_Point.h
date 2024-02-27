#pragma once

#include "ZCR_Color.h"
#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>

#include <list>

struct ZCR_Point : public ZCR_Color
{
    ZCR_Point();

    ZC_sptr<std::list<Points>> spPoints;    //  fill in GetPoints
    std::list<Points*> activePoints {};

    ZC_sptr<ZC_RendererSet> spRendererSetsPoint;
    ZC_sptr<ZC_RendererSetAndDrawingSet> spRSADSPoint;

    ZC_sptr<ZC_RendererSet> MakePointRendererSet();
    ZC_DA<uchar> GetPointElements(size_t& rElementsCount, GLenum& rElementsType);
    void GetPoints(size_t& rElementsCount);
    size_t FillPoints(ZC_Vec3<float>* pVertex, bool isQuad, ZC_Vec3<float>* pVertexContainerHead);
    template<typename TElement>
    void FillPointElements(TElement* pIndex);

    typedef typename ZC_RendererSet::Level RSLvl;
    void SwitchRSandDSPoint(RSLvl lvl);

    void MakePointsActive(std::list<Points*>&& points);
};


template<typename TElement>
void ZCR_Point::FillPointElements(TElement* pElement)
{
    size_t trianglesStartOffsetInEBO = spQuads ? (spQuads->size * 4) : 0;    //  triangles start index in ebo
    size_t pElementI = 0;
    auto pQuadsHead = spQuads ? &(spQuads->Begin()->bl) : nullptr;
    auto pTrianglesHead = spTriangles ? &(spTriangles->Begin()->bl) : nullptr;
    for (auto& point : *spPoints)
    {
        auto& samePoint = point.GetSamePoint();
        pElement[pElementI++] = samePoint.isQuad ?
            static_cast<TElement>(samePoint.pVertex - pQuadsHead)
            : static_cast<TElement>((samePoint.pVertex - pTrianglesHead) + trianglesStartOffsetInEBO);
    }
}