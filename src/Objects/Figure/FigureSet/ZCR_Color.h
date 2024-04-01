#pragma once

#include "ZCR_VBO.h"

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

    void FillColorsAllAsPassive(bool needBufferData);
    void FillColorsAllAsActive(bool needBufferData);
    void FillColorActivePoint(Points* pPoints);

private:
    static inline const ZC_Vec3<uchar> colorPassivePoint { 0, 0, 0 };        //  black
    static inline const ZC_Vec3<uchar> colorActivePoint { 253, 127, 57 };    //  orange

    ulong trianglesStartVertexIndex = spQuads->size * 4;
};