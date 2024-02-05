#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>
#include <ZC/Video/OpenGL/Figure/ZC_Quad.h>

#include <list>

class ZCR_Point
{
public:
    ZC_RSNonTex rendSet;
    ZC_Uniforms* pUniforms = nullptr;
    // std::list<ZC_Vec3<float>> points;

    ZCR_Point(ZC_DA<ZC_Quad>& quads, ZC_Buffer& vbo, const ZC_Mat4<float>& model);

    ZCR_Point(ZCR_Point&& p);

private:
    struct Point
    {
        std::forward_list<ZC_Vec3<float>*> samePoints;

        bool operator == (const ZC_Vec3<float>& _v) const noexcept;

        ZC_Vec3<float>* GetPointerOnPoint();
    };

    ZC_RSNonTex MakeRendSet(ZC_DA<ZC_Quad>& quads, ZC_Buffer& vbo);
    ZC_DA<unsigned char> GetElements(ZC_DA<ZC_Quad>& quads, size_t& rElementsCount, GLenum& rElementsType);
    std::list<Point> GetPoints(ZC_DA<ZC_Quad>& quads);

    template<typename TIndex>
    void FillElements(typename std::list<Point>& points, TIndex* pIndex, ZC_Vec3<float>* pQuadsHead);
};


template<typename TIndex>
void ZCR_Point::FillElements(typename std::list<Point>& points, TIndex* pIndex, ZC_Vec3<float>* pQuadsHead)
{
    size_t pIndexI = 0;
    for (auto pointsIter = points.begin(); pointsIter != points.end(); ++pointsIter, ++pIndexI)
    {
        pIndex[pIndexI] = static_cast<TIndex>(pointsIter->GetPointerOnPoint() - pQuadsHead);
    }
}