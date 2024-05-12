#pragma once

#include "ZCR_Color.h"
#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>

#include <list>

struct ZCR_Point : public ZCR_Color
{
    std::list<Points> points;    //  fill in GetPoints
    std::list<Points*> activePoints;

    ZC_DrawerSet dsPoint;
    ZC_DSController dsConPoint;

    ZCR_Point();
    ZCR_Point(ZCR_Point&& p);

    void MakePointsActive(std::list<Points*>& points);
    void MakeAllPointsActive();
    void MakeAllPointsPassive();
    void SwitchRSControllerPoint(ZC_DrawerLevel drawerLevel);

private:
    ZC_DrawerSet CreatePointDrawerSet();
    ZC_DA<uchar> GetPointElements(ulong& rElementsCount, GLenum& rElementsType);
    void GetPoints(ulong& rElementsCount);
    ulong FillPoints(ZC_Vec3<float>* pVertex, bool isQuad, ZC_Vec3<float>* pVertexContainerHead);
    template<typename TElement>
    void FillPointElements(TElement* pIndex);
};


template<typename TElement>
void ZCR_Point::FillPointElements(TElement* pElement)
{
    ulong trianglesStartOffsetInEBO = quads.size * 4;    //  triangles start index in ebo
    ulong pElementI = 0;
    auto pQuadsHead = &(quads.Begin()->bl);
    auto pTrianglesHead = &(triangles.Begin()->bl);
    for (auto& point : points)
    {
        auto& samePoint = point.GetSamePoint();
        pElement[pElementI++] = samePoint.isQuad ?
            static_cast<TElement>(samePoint.pVertex - pQuadsHead)
            : static_cast<TElement>((samePoint.pVertex - pTrianglesHead) + trianglesStartOffsetInEBO);
    }
}