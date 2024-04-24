#pragma once

#include "ZCR_TextAxis.h"

struct ZCR_TextAxises
{
    ZCR_TextAxis* pActiveText = nullptr;
    ZCR_TextAxis texts[6]
    {
        ZCR_TextAxis(ZCR_A_PositiveX, { 1.f, 0.f, 0.f }, "+X", 225, 0, 0),
        ZCR_TextAxis(ZCR_A_NegativeX, { -1.f, 0.f, 0.f }, "-X", 180, 0, 0),
        ZCR_TextAxis(ZCR_A_PositiveY, { 0.f, 1.f, 0.f }, "+Y", 0, 225, 0),
        ZCR_TextAxis(ZCR_A_NegativeY, { 0.f, -1.f, 0.f }, "-Y", 0, 180, 0),
        ZCR_TextAxis(ZCR_A_PositiveZ, { 0.f, 0.f, 1.f }, "+Z", 0, 180, 225),
        ZCR_TextAxis(ZCR_A_NegativeZ, { 0.f, 0.f, -1.f }, "-Z", 0, 155, 225)
    };

    void SetPosition(const ZC_Vec3<float>& newPosition);
    void CalculateQuads(const ZC_Mat4<float>& perspView, const ZC_Vec2<float>& indents, float textureWidth, float textureHeight);
    //  returns true if need redraw texture (were changed pActiveText)
    bool MakeCursorMoveCollision();
    void LeaveActiveArea();
    void MouseLeftButtonDown();
};
