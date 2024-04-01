#include "ZCR_Color.h"

void ZCR_Color::FillColorsAllAsPassive(bool needBufferData)
{
    std::fill(&((*(colors.pHead))[0]), &((*(colors.pHead + (colors.size - 1)))[2]), 0); //  dangerous place
    if (needBufferData) BufferSubDataAllStoredType(StoredType_Color, colors.pHead, false);
}

void ZCR_Color::FillColorsAllAsActive(bool needBufferData)
{
    for (ulong i = 0; i < colors.size; ++i) colors[i] = colorActivePoint;
    if (needBufferData) BufferSubDataAllStoredType(StoredType_Color, colors.pHead, false);
}

void ZCR_Color::FillColorActivePoint(Points* pPoints)
{
    for (auto& samePoint : pPoints->samePoints)
    {
        colors[samePoint.isQuad ? samePoint.indexInQuadsOrTriangles : trianglesStartVertexIndex + samePoint.indexInQuadsOrTriangles] = colorActivePoint;
        BufferSubDataIndex(StoredType_Color, samePoint.isQuad, samePoint.indexInQuadsOrTriangles, &colorActivePoint);
    }
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