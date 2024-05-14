#pragma once

#include "ZCR_Color.h"
#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>
#include <Objects/Scene/ZCR_SceneMode.h>

class ZCR_Line : public virtual ZCR_Color
{
public:
    //  returns true if lines drawing in all scene modes
    bool IsDrawLines();
    //  sets to draw lines in all scene modes
    void SetDrawLines(bool _drawLines);

private:
    ZC_DrawerSet dsLine;

protected:
    ZC_DSController dscLine;

    ZCR_Line();
    // ZCR_Line(ZCR_Line&& l);

    void ChangeSceneModeLine(ZCR_SceneMode sceneMode);

private:
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

    bool drawLines = false;

    ZC_DrawerSet CreateLineDrawerSet();
    ZC_DA<uchar> GetLineElements(ulong& rElementsCount, GLenum& rElementsType);
    std::forward_list<Lines> GetLines(ulong& rElementsCount);
    template<typename TElement>
    void FillLineElements(std::forward_list<Lines>& lines, TElement* pElement);
    // ZC_DSController ZCR_Line::MakeSurfaceRSController();
};

template<typename TElement>
void ZCR_Line::FillLineElements(typename std::forward_list<Lines>& lines, TElement* pElement)
{
    ulong trianglesStartOffsetInEBO = quads.size * 4;    //  triangles start index in ebo
    ulong pElementI = 0;
    auto pQuadsHead = &(quads.Begin()->bl);
    auto pTrianglesHead = &(triangles.Begin()->bl);
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