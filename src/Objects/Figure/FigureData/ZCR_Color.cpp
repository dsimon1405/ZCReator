#include "ZCR_Color.h"

// ZCR_Color::ZCR_Color(ZC_DA<ZC_Quad>&& _quads, ZC_DA<ZC_Triangle>&& _triangles, ZC_DA<int>&& _normals)
//     : ZCR_VBO(std::move(_quads), std::move(_triangles), { (quads.size * 4) + (triangles.size * 3) }, std::move(_normals))
// {}

void ZCR_Color::FillColorsAllAsPassive(bool bufferData)
{
    std::fill(colors.pHead, colors.pHead + colors.size, colorPassivePoint);
    if (bufferData)
    {
        this->BufferSubDataAllStoredType(ST_Color);
        colorStateGPU = CS_GPU_AllPassive;
    }
}

void ZCR_Color::FillColorsAllAsActive(bool bufferData)
{
    std::fill(colors.pHead, colors.pHead + colors.size, colorActivePoint);
    if (bufferData)
    {
        this->BufferSubDataAllStoredType(ST_Color);
        colorStateGPU = CS_GPU_AllActive;
    }
}

void ZCR_Color::FillColorActivePoint(Points* pPoints)
{
    for (auto& samePoint : pPoints->samePoints)
    {
        colors[samePoint.isQuad ? samePoint.indexInQuadsOrTriangles : trianglesStartVertexIndex + samePoint.indexInQuadsOrTriangles] = colorActivePoint;
        this->BufferSubDataIndex(ST_Color, samePoint.isQuad, samePoint.indexInQuadsOrTriangles, &colorActivePoint);
        colorStateGPU = CS_GPU_ActivePoint;
    }
}

void ZCR_Color::FillColorActivePoints(const std::list<Points*>& _activePoints, bool passiveClear, bool bufferData)
{
    if (_activePoints.empty()) return;
    
    if (passiveClear) FillColorsAllAsPassive(false);
    
    for (Points* pActivePoint : _activePoints)
        for (auto& samePoint : pActivePoint->samePoints)
            colors[samePoint.isQuad ? samePoint.indexInQuadsOrTriangles : trianglesStartVertexIndex + samePoint.indexInQuadsOrTriangles] = colorActivePoint;

    if (bufferData)
    {
        this->BufferSubDataAllStoredType(ST_Color);
        colorStateGPU = CS_GPU_ActivePoint;
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