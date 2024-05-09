#include "ZCR_TextAxis.h"

#include <Renderer/ZCR_RenderLevel.h>
#include <Objects/Scene/ZCR_Scene.h>

ZCR_TextAxis::ZCR_TextAxis(ZCR_Axis _axis, const ZC_Vec3<float>& _startPosition, const std::string& text, uint _defaultColor)
    : axis(_axis),
    startPosition(_startPosition),
    textWindowIntoScene({ ZC_FontName::ZC_F_ChunkFivePrint, 18 }, ZC_FO_center, text, ZC_TA_Center, {}, false),
    defaultColor(_defaultColor)
{
    textWindowIntoScene.SetFrameBuffer(ZCR_RL_Orientation3D);
    textWindowIntoScene.SetColorUInt(defaultColor);
    textWindowIntoScene.NeedDraw(true);
}

void ZCR_TextAxis::SetPosition(const ZC_Vec3<float>& newPosition)
{
    textWindowIntoScene.SetPosition(startPosition + newPosition);
}

void ZCR_TextAxis::CalculateQuad(const ZC_Mat4<float>& perspView, const ZC_Vec2<float>& windowIndents, float textureWidth, float textureHeight)
{
    auto position = perspView * ZC_Vec4<float>(textWindowIntoScene.GetPosition(), 1.f);
    float posX = position[0] / position[3];     //  [-1,1]
    posX = ZC_ToZeroOneRange(posX);     //  [0,1]
    posX *= textureWidth;    //  pixel position in bottom left corner of the window
    posX += windowIndents[0];    //  final origin position
    float posY = position[1] / position[3];     //  [-1,1]
    posY = ZC_ToZeroOneRange(posY);     //  [0,1]
    posY *= textureHeight;    //  pixel position in bottom left corner of the window
    posY += windowIndents[1];    //  final origin position
    float textHalfWidth = textWindowIntoScene.GetWidth() / 2.f,
        textHalfHeight = textWindowIntoScene.GetHeight() / 2.f;
    
    blX = posX - textHalfWidth;
    blY = posY - textHalfHeight;
    trX = posX + textHalfWidth;
    trY = posY + textHalfHeight;
    depth = position[2] / position[3];
}

bool ZCR_TextAxis::MakeCursorCollision(float cursorX, float cursorY, float activeDepth)
{
    return blX < cursorX && blY < cursorY && trX > cursorX && trY > cursorY && depth < activeDepth;
}

void ZCR_TextAxis::SetState(State newState)
{
    if (state == newState) return;

    static const uint colorActiveAxis = ZC_PackColorUCharToUInt(255, 255, 255);   //  white color
    static const uint colorPressedAxis = ZC_PackColorUCharToUInt(127, 127, 127);   //  grey color

    //  in current state need change only drawer level if current state is don't uses depth test
    if (state == S_Pressed || (state == S_UnderCursor && newState != S_Pressed))
        textWindowIntoScene.SetDrawerLevel(ZC_DrawerLevels::TextWindowIntoScene);
    
    switch (newState) //  set new state
    {
    case S_Default: textWindowIntoScene.SetColorUInt(defaultColor); break;
    case S_UnderCursor:
    {
        textWindowIntoScene.SetDrawerLevel(ZC_DrawerLevels::OrthoBlend);
        textWindowIntoScene.SetColorUInt(0);      //  black color
    } break;
    case S_Pressed: textWindowIntoScene.SetColorUInt(colorPressedAxis); break;
    case S_Active: textWindowIntoScene.SetColorUInt(colorActiveAxis); break;
    }

    state = newState;
}

void ZCR_TextAxis::MouseLeftButtonUp()
{
    ZCR_Scene::GetScene()->SetAxise(axis);
    SetState(S_Active);
}