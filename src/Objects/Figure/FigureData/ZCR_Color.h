#pragma once

#include "ZCR_VBO.h"

#include <forward_list>
#include <list>

struct ZCR_Color : public virtual ZCR_VBO
{
protected:
    struct Points
    {
        struct SamePoint
        {
            ZC_Vec3<float>* pVertex;
            bool isQuad;
            ulong indexInQuadsOrTriangles;
        };

        std::forward_list<SamePoint> samePoints;

        bool operator == (const ZC_Vec3<float>* _v) const noexcept;
        
        //  returns last added Same Point, with more chanse it will be from the quads, cause quads adds after triangles in forward_list
        SamePoint& GetSamePoint();
    };

    enum ColorStateGPU
    {
        CS_GPU_AllPassive,
        CS_GPU_AllActive,
        CS_GPU_ActivePoint
    };

    std::list<Points*> activePoints;
    ColorStateGPU colorStateGPU;

    // ZCR_Color(ZC_DA<ZC_Quad>&& _quads, ZC_DA<ZC_Triangle>&& _triangles, ZC_DA<int>&& _normals);

    void FillColorsAllAsPassive(bool bufferData);
    void FillColorsAllAsActive(bool bufferData);
    void FillColorActivePoint(Points* pPoints);

    /*
    Params:
    - _activePoints - point to make active.
    - passiveClear - is it necessary to clear the point color buffer with a passive color before coloring the active points?
    - bufferData - need buffer data on gpu?
    */
    void FillColorActivePoints(const std::list<Points*>& _activePoints, bool passiveClear, bool bufferData);

private:
    static inline const ZC_Vec3<uchar> colorPassivePoint { 0, 0, 0 };        //  black
    static inline const ZC_Vec3<uchar> colorActivePoint { 253, 127, 57 };    //  orange

    ulong trianglesStartVertexIndex = quads.size * 4;
};