#pragma once

#include "ZCR_VBO.h"
#include <ZC/Video/OpenGL/Renderer/ZC_RenderSet.h>

struct ZCR_Line : public virtual ZCR_VBO
{
    ZCR_Line();

    ZC_sptr<ZC_RenderSet> spRendererSetsLine;
    ZC_RSController rsControllerLine;

    struct Lines
    {
        struct Corners
        {
            ZC_Vec3<float>* pCorner1,
                * pCorner2;
            bool isQuad;
        };

        ZC_Vec3<float> edgeCenter;
        std::forward_list<Corners> corners;

        bool operator == (const ZC_Vec3<float>& _edgeCenter) const noexcept;
        Corners& GetCorners();
    };

    ZC_sptr<ZC_RenderSet> MakeLineRendererSet();
    ZC_DA<uchar> GetLineElements(ulong& rElementsCount, GLenum& rElementsType);
    std::forward_list<Lines> GetLines(ulong& rElementsCount);
    template<typename TElement>
    void FillLineElements(std::forward_list<Lines>& lines, TElement* pElement);

    void SwitchRSControllerLine(ZC_DrawLevel drawLevel);
};

template<typename TElement>
void ZCR_Line::FillLineElements(typename std::forward_list<Lines>& lines, TElement* pElement)
{
    ulong trianglesStartOffsetInEBO = spQuads ? (spQuads->size * 4) : 0;    //  triangles start index in ebo
    ulong pElementI = 0;
    auto pQuadsHead = spQuads ? &(spQuads->Begin()->bl) : nullptr;
    auto pTrianglesHead = spTriangles ? &(spTriangles->Begin()->bl) : nullptr;
    for (auto& line : lines)
    {
        auto& corners = line.GetCorners();
        if (corners.isQuad)
        {
            pElement[pElementI++] = static_cast<TElement>(corners.pCorner1 - pQuadsHead);
            pElement[pElementI++] = static_cast<TElement>(corners.pCorner2 - pQuadsHead);
        }
        else
        {
            pElement[pElementI++] = static_cast<TElement>((corners.pCorner1 - pTrianglesHead) + trianglesStartOffsetInEBO);
            pElement[pElementI++] = static_cast<TElement>((corners.pCorner2 - pTrianglesHead) + trianglesStartOffsetInEBO);
        }
    }
}