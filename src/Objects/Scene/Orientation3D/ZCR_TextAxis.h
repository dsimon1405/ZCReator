#pragma once

#include <Objects/Scene/ZCR_Axis.h>
#include <ZC/Objects/Text/ZC_TextWindowIntoScene.h>
#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Tools/Math/ZC_Vec2.h>

struct ZCR_TextAxis
{
    ZCR_Axis axis;
    ZC_Vec3<float> startPosition;
    ZC_TextWindowIntoScene textWindowIntoScene;
    uchar red,
        green,
        blue;
    float blX,  //  bottom left corner
        blY,
        trX,    //  top right corner
        trY,
        depth;

    ZCR_TextAxis(ZCR_Axis _axis, const ZC_Vec3<float>& _startPosition, const std::string& text, uchar _red, uchar _green, uchar _blue);
    void SetPosition(const ZC_Vec3<float>& newPosition);
    void CalculateQuad(const ZC_Mat4<float>& perspView, const ZC_Vec2<float>& windowIndents, float textureWidth, float textureHeight);
    //  if depth less then activeDepth (closer to camera) and cursor in text area returns true
    bool MakeCursorCollision(float cursorX, float cursorY, float activeDepth);
    void SetColor(bool setWhite);
    void MouseLeftButtonDown();
};