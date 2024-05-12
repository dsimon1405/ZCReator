#include "ZCR_Color.h"

void ZCR_Color::FillColorsAllAsPassive(bool bufferData)
{
    std::fill(&((*(colors.pHead))[0]), &((*(colors.pHead + (colors.size - 1)))[2]), 0); //  dangerous place
    if (bufferData) BufferSubDataAllStoredType(ST_Color);
}

void ZCR_Color::FillColorsAllAsActive(bool bufferData)
{
    for (ulong i = 0; i < colors.size; ++i) colors[i] = colorActivePoint;
    if (bufferData) BufferSubDataAllStoredType(ST_Color);
}

void ZCR_Color::FillColorActivePoint(Points* pPoints)
{
    for (auto& samePoint : pPoints->samePoints)
    {
        colors[samePoint.isQuad ? samePoint.indexInQuadsOrTriangles : trianglesStartVertexIndex + samePoint.indexInQuadsOrTriangles] = colorActivePoint;
        BufferSubDataIndex(ST_Color, samePoint.isQuad, samePoint.indexInQuadsOrTriangles, &colorActivePoint);
    }
}

void ZCR_Color::FillColorsActivePoints(const std::list<Points*>& _activePoints, bool passiveClear, bool bufferData)
{
    if (passiveClear) FillColorsAllAsPassive(false);
    
    for (Points* pActivePoint : _activePoints)
        for (auto& samePoint : pActivePoint->samePoints)
            colors[samePoint.isQuad ? samePoint.indexInQuadsOrTriangles : trianglesStartVertexIndex + samePoint.indexInQuadsOrTriangles] = colorActivePoint;

    if (bufferData) this->BufferSubDataAllStoredType(ST_Color);
}


//  Points start

bool ZCR_Color::Points::operator == (const ZC_Vec3<float>* _v) const noexcept
{
    return *(samePoints.begin()->pVertex) == *_v;
}

typename ZCR_Color::Points::SamePoint& ZCR_Color::Points::GetSamePoint()
{
    return *samePoints.begin();
}