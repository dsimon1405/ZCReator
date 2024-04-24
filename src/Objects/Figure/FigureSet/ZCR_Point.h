#pragma once

#include "ZCR_Color.h"
#include <ZC/Video/OpenGL/Renderer/ZC_RenderSet.h>

#include <list>

struct ZCR_Point : public ZCR_Color
{
    ZCR_Point();

    ZC_sptr<std::list<Points>> spPoints;    //  fill in GetPoints
    std::list<Points*> activePoints {};

    ZC_sptr<ZC_RenderSet> spRendererSetsPoint;
    ZC_RSController rsControllerPoint;

    ZC_sptr<ZC_RenderSet> MakePointRenderSet();
    ZC_DA<uchar> GetPointElements(ulong& rElementsCount, GLenum& rElementsType);
    void GetPoints(ulong& rElementsCount);
    ulong FillPoints(ZC_Vec3<float>* pVertex, bool isQuad, ZC_Vec3<float>* pVertexContainerHead);
    template<typename TElement>
    void FillPointElements(TElement* pIndex);

    void SwitchRSControllerPoint(ZC_DrawLevel drawLevel);

    void MakePointsActive(std::list<Points*>&& points);
};


template<typename TElement>
void ZCR_Point::FillPointElements(TElement* pElement)
{
    ulong trianglesStartOffsetInEBO = spQuads ? (spQuads->size * 4) : 0;    //  triangles start index in ebo
    ulong pElementI = 0;
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