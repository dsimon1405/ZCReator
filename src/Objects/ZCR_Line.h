#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>
#include <ZC/Video/OpenGL/Figure/ZC_Quad.h>

#include <list>

class ZCR_Line
{
public:
    ZC_RSNonTex rendSet;
    ZC_Uniforms* pUniforms = nullptr;
    // std::list<ZC_Vec3<float>> points;

    ZCR_Line(ZC_DA<ZC_Quad>& quads, ZC_Buffer& vbo, const ZC_Mat4<float>& model);

    ZCR_Line(ZCR_Line&& p);

private:
    struct Line
    {
        struct Corners
        {
            ZC_Vec3<float>* pCorner1,
                * pCorner2;
        };

        ZC_Vec3<float> edgeCenter;
        std::forward_list<Corners> corners;

        bool operator == (const ZC_Vec3<float>& _edgeCenter) const noexcept;
    };

    ZC_RSNonTex RendSet(ZC_DA<ZC_Quad>& quads, ZC_Buffer& vbo);
    ZC_DA<unsigned char> GetElements(ZC_DA<ZC_Quad>& quads, size_t& rElementsCount, GLenum& rElementsType);
    typename std::list<Line> GetLines(ZC_DA<ZC_Quad>& quads);

    template<typename TElement>
    void FillElements(typename std::list<Line>& lines, TElement* pElement, ZC_Vec3<float>* pQuadsHead);
};


template<typename TElement>
void ZCR_Line::FillElements(typename std::list<Line>& lines, TElement* pElement, ZC_Vec3<float>* pQuadsHead)
{
    size_t pElementI = 0;
    for (auto linesIter = lines.begin(); linesIter != lines.end(); ++linesIter)
    {
        auto cornersIter = linesIter->corners.begin();
        pElement[pElementI++] = static_cast<TElement>(cornersIter->pCorner1 - pQuadsHead);
        pElement[pElementI++] = static_cast<TElement>(cornersIter->pCorner2 - pQuadsHead);
    }
}