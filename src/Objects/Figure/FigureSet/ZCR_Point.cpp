#include "ZCR_Point.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>

ZCR_Point::ZCR_Point()
    : spRendererSetsPoint(MakePointRendererSet()),
    rsControllerPoint(spRendererSetsPoint->MakeZC_RSController())
{}

ZC_sptr<ZC_RendererSet> ZCR_Point::MakePointRendererSet()
{
    size_t elementsCount = 0;
    GLenum elementsType = 0;
    ZC_DA<uchar> elements = GetPointElements(elementsCount, elementsType);

    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);

    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(GL_POINTS, elementsCount, elementsType, 0);

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::ZCR_Point);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, *vbo, &ebo, 0, (spQuads ? (spQuads->size * 4) : 0) + (spTriangles ? (spTriangles->size * 3) : 0));   //  vertices count in vbo (in one quad 4, in one triangle 3)

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(ebo));

    return ZC_RendererSet::CreateShptr(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers));
}

ZC_DA<uchar> ZCR_Point::GetPointElements(size_t& rElementsCount, GLenum& rElementsType)
{
    GetPoints(rElementsCount);
    size_t storingTypeSize = 0;
    ZC_Buffer::GetElementsData(rElementsCount - 1, storingTypeSize, rElementsType);
    ZC_DA<uchar> elements(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillPointElements(elements.pHead); break;
    case 2: FillPointElements(reinterpret_cast<ushort*>(elements.pHead)); break;
    case 4: FillPointElements(reinterpret_cast<uint*>(elements.pHead)); break;
    }
    return elements;
}

void ZCR_Point::GetPoints(size_t& rElementsCount)
{
    spPoints = std::move(ZC_sptr<std::list<Points>>(new std::list<Points>()));

    auto pVertexHeadOfTriangles = spTriangles ? &(spTriangles->Begin()->bl) : nullptr;
    for (size_t i = 0; i < (spTriangles ? spTriangles->size : 0); ++i)
    {
        rElementsCount += FillPoints(&((*spTriangles)[i].bl), false, pVertexHeadOfTriangles);
        rElementsCount += FillPoints(&((*spTriangles)[i].tc), false, pVertexHeadOfTriangles);
        rElementsCount += FillPoints(&((*spTriangles)[i].br), false, pVertexHeadOfTriangles);  
    }
    auto pVertexHeadOfQuads = spQuads ? &(spQuads->Begin()->bl) : nullptr;
    for (size_t i = 0; i < (spQuads ? spQuads->size : 0); ++i)
    {
        rElementsCount += FillPoints(&((*spQuads)[i].bl), true, pVertexHeadOfQuads);
        rElementsCount += FillPoints(&((*spQuads)[i].tl), true, pVertexHeadOfQuads);
        rElementsCount += FillPoints(&((*spQuads)[i].tr), true, pVertexHeadOfQuads);
        rElementsCount += FillPoints(&((*spQuads)[i].br), true, pVertexHeadOfQuads);
    }
}

size_t ZCR_Point::FillPoints(ZC_Vec3<float>* pVertex, bool isQuad, ZC_Vec3<float>* pVertexContainerHead)
{
    auto pPoints = ZC_Find(*spPoints, pVertex);
    if(pPoints)
    {
        pPoints->samePoints.emplace_front(Points::SamePoint{ pVertex, isQuad, static_cast<size_t>(pVertex - pVertexContainerHead) });
        return 0;   //  in ebo point is one value, if found same return 0
    }
    else
    {
        spPoints->emplace_back(Points{ { Points::SamePoint{ pVertex, isQuad, static_cast<size_t>(pVertex - pVertexContainerHead) } } });
        return 1;   //  in ebo point is one value, if not found same return 1 (new point)
    }
}

void ZCR_Point::SwitchRSandDSPoint(ZC_RendererLevel lvl)
{
    rsControllerPoint.SwitchToLvl(lvl);
}

void ZCR_Point::MakePointsActive(std::list<Points*>&& points)
{
    if (points.size() == spPoints->size() && points.size() == activePoints.size()) return;
    if (points.size() == spPoints->size())
    {
        FillColorsAllAsActive(true);
        activePoints = std::move(points);
    }
    else
    {
        for (auto pPoint : points)
        {
            FillColorActivePoint(pPoint);
            activePoints.emplace_back(pPoint);
        }
    }
}