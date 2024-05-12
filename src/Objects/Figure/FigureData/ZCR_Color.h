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

    void FillColorsAllAsPassive(bool bufferData);
    void FillColorsAllAsActive(bool bufferData);
    void FillColorActivePoint(Points* pPoints);

    /*
    Params:
    _activePoints - point to make active.
    passiveClear - need to clear points color buffer in passive color, before make active points?
    bufferData - need buffer data on gpu?
    */
    void FillColorsActivePoints(const std::list<Points*>& _activePoints, bool passiveClear, bool bufferData);

private:
    static inline const ZC_Vec3<uchar> colorPassivePoint { 0, 0, 0 };        //  black
    static inline const ZC_Vec3<uchar> colorActivePoint { 253, 127, 57 };    //  orange

    ulong trianglesStartVertexIndex = quads.size * 4;
};