#include "ZCR_Point.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>

void ZCR_Point::MakePointActive(Points* _point)
{
    if (ZC_Find(this->activePoints, _point)) return;  //  point allready active
    this->activePoints.emplace_back(_point);
    this->FillColorActivePoint(_point);
}

void ZCR_Point::MakePointsActive(std::list<Points*>& _points)
{
    if (points.size() == _points.size() && _points.size() == this->activePoints.size()) return;
    if (points.size() == _points.size()) MakeAllPointsActive();
    else
    {
        for (auto _pointsIter = _points.begin(); _pointsIter != _points.end(); )
        {
            if (ZC_Find(this->activePoints, *_pointsIter)) _pointsIter = _points.erase(_pointsIter);  //  point allready active
            else
            {
                this->activePoints.emplace_back(*_pointsIter);
                ++_pointsIter;
            }
        }
        if (!(_points.empty())) this->FillColorActivePoints(_points, false, true);
    }
}

void ZCR_Point::MakeAllPointsActive()
{
    this->activePoints.clear();
    for (auto& point : points) this->activePoints.emplace_back(&point);
    this->FillColorsAllAsActive(true);
}

void ZCR_Point::MakeAllPointsPassive()
{
    this->activePoints.clear();
    this->FillColorsAllAsPassive(true);
}

ZCR_Point::ZCR_Point()
    : points{},
    dsPoint(CreatePointDrawerSet()),
    dscPoint(dsPoint.MakeZC_DSController())
{}

// ZCR_Point::ZCR_Point(ZCR_Point&& p)
//     : points(std::move(p.points)),
//     dsPoint(std::move(p.dsPoint)),
//     dsConPoint(dsPoint.MakeZC_DSController())
// {
//     dsConPoint.SwitchToDrawLvl(ZC_RL_Default, p.dsConPoint.GetDrawingLevel(ZC_RL_Default));   //  switch new controller to acoding level of previous controller
// }

void ZCR_Point::ChangeSceneModePoint(ZCR_SceneMode sceneMode)
{
    sceneMode == ZCR_SM_Edit && isActiveOnScene ? dscPoint.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing) : dscPoint.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None);
}

ZC_DrawerSet ZCR_Point::CreatePointDrawerSet()
{
    ulong elementsCount = 0;
    GLenum elementsType = 0;
    ZC_DA<uchar> elements = GetPointElements(elementsCount, elementsType);

    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);

    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(GL_POINTS, elementsCount, elementsType, 0);

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ShPN_ZCR_Point);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, &ebo, 0, (quads.size * 4) + (triangles.size * 3));   //  vertices count in vbo (in one quad 4, in one triangle 3)

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(ebo));

    return { pShPIS, std::move(vao), std::move(upDraw), std::move(buffers) };
}

ZC_DA<uchar> ZCR_Point::GetPointElements(ulong& rElementsCount, GLenum& rElementsType)
{
    GetPoints(rElementsCount);
    ulong storingTypeSize = 0;
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

void ZCR_Point::GetPoints(ulong& rElementsCount)
{
    auto lambFillPoints = [this](ZC_Vec3<float>* pVertex, bool isQuad, ZC_Vec3<float>* pVertexContainerHead) -> ulong
    {
        auto pPoints = ZC_Find(points, pVertex);
        if(pPoints)
        {
            pPoints->samePoints.emplace_front(Points::SamePoint{ pVertex, isQuad, static_cast<ulong>(pVertex - pVertexContainerHead) });
            return 0;   //  in ebo point is one value, if found same return 0
        }
        else
        {
            points.emplace_back(Points{ { Points::SamePoint{ pVertex, isQuad, static_cast<ulong>(pVertex - pVertexContainerHead) } } });
            return 1;   //  in ebo point is one value, if not found same return 1 (new point)
        }
    };

    auto pVertexHeadOfTriangles = &(triangles.Begin()->bl);
    for (ulong i = 0; i < triangles.size; ++i)
    {
        rElementsCount += lambFillPoints(&(triangles[i].bl), false, pVertexHeadOfTriangles);
        rElementsCount += lambFillPoints(&(triangles[i].tc), false, pVertexHeadOfTriangles);
        rElementsCount += lambFillPoints(&(triangles[i].br), false, pVertexHeadOfTriangles);  
    }
    auto pVertexHeadOfQuads = &(quads.Begin()->bl);
    for (ulong i = 0; i < quads.size; ++i)
    {
        rElementsCount += lambFillPoints(&(quads[i].bl), true, pVertexHeadOfQuads);
        rElementsCount += lambFillPoints(&(quads[i].tl), true, pVertexHeadOfQuads);
        rElementsCount += lambFillPoints(&(quads[i].tr), true, pVertexHeadOfQuads);
        rElementsCount += lambFillPoints(&(quads[i].br), true, pVertexHeadOfQuads);
    }
}