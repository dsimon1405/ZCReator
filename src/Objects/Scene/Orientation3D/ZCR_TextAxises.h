#pragma once

#include "ZCR_TextAxis.h"

struct ZCR_TextAxises
{
    ZCR_TextAxis* pActiveMoveText = nullptr;
    ZCR_TextAxis* pActivePressedText = nullptr;
    ZCR_TextAxis* pActiveAxisText = nullptr;
    bool isButtonPressed = false;
    ZCR_TextAxis texts[6]
    {
        ZCR_TextAxis(ZCR_A_PositiveX, { 1.f, 0.f, 0.f }, "+X", ZC_PackColorUCharToUInt(225, 0, 0)),
        ZCR_TextAxis(ZCR_A_NegativeX, { -1.f, 0.f, 0.f }, "-X", ZC_PackColorUCharToUInt(180, 0, 0)),
        ZCR_TextAxis(ZCR_A_PositiveY, { 0.f, 1.f, 0.f }, "+Y", ZC_PackColorUCharToUInt(0, 225, 0)),
        ZCR_TextAxis(ZCR_A_NegativeY, { 0.f, -1.f, 0.f }, "-Y", ZC_PackColorUCharToUInt(0, 180, 0)),
        ZCR_TextAxis(ZCR_A_PositiveZ, { 0.f, 0.f, 1.f }, "+Z", ZC_PackColorUCharToUInt(0, 180, 225)),
        ZCR_TextAxis(ZCR_A_NegativeZ, { 0.f, 0.f, -1.f }, "-Z", ZC_PackColorUCharToUInt(0, 155, 225)),
    };
    bool makeActiveAxisDefault = true;

    void SetPosition(const ZC_Vec3<float>& newPosition);
    void CalculateQuads(const ZC_Mat4<float>& perspView, const ZC_Vec2<float>& indents, float textureWidth, float textureHeight);
    //  returns true if need redraw texture (were changed pActiveText)
    bool MakeCursorMoveCollision();
    //  returns true if need redraw texture (were changed pActiveText)
    bool LeaveActiveArea();
    bool MouseLeftButtonDown();
    void MouseLeftButtonUp();
    void PrepareReconnect();
    void CamMoveWhileDeactivated();
};
