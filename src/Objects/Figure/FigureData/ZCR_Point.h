#pragma once

#include "ZCR_Color.h"
#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>
#include <Objects/Scene/ZCR_SceneMode.h>

#include <list>

class ZCR_Point : public virtual ZCR_Color
{
public:
    void MakePointActive(Points* _point);
    void MakePointsActive(std::list<Points*>& points);
    void MakeAllPointsActive();
    void MakeAllPointsPassive();

private:
    std::list<Points> points;    //  fills in GetPoints
    ZC_DrawerSet dsPoint;

protected:
    ZC_DSController dscPoint;

    ZCR_Point();
    // ZCR_Point(ZCR_Point&& p);

    void ChangeSceneModePoint(ZCR_SceneMode sceneMode);

private:
    ZC_DrawerSet CreatePointDrawerSet();
    ZC_DA<uchar> GetPointElements(ulong& rElementsCount, GLenum& rElementsType);
    void GetPoints(ulong& rElementsCount);
    // ulong FillPoints(ZC_Vec3<float>* pVertex, bool isQuad, ZC_Vec3<float>* pVertexContainerHead);
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