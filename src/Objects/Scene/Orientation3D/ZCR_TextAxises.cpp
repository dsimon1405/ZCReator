#include "ZCR_TextAxises.h"

#include <ZC/Video/ZC_SWindow.h>

void ZCR_TextAxises::SetPosition(const ZC_Vec3<float>& newPosition)
{
    if (pActiveAxisText)
    {
        if (makeActiveAxisDefault)
        {
            pActiveAxisText->SetState(ZCR_TextAxis::S_Default);
            pActiveAxisText = nullptr;
        }
        makeActiveAxisDefault = true;
    }
    for (auto& text : texts) text.SetPosition(newPosition);
}

void ZCR_TextAxises::CalculateQuads(const ZC_Mat4<float>& perspView, const ZC_Vec2<float>& indents, float textureWidth, float textureHeight)
{
    for (auto& text : texts) text.CalculateQuad(perspView, indents, textureWidth, textureHeight);
}

bool ZCR_TextAxises::MakeCursorMoveCollision()
{
    float cursorX,
        cursorY;
    ZC_SWindow::GetCursorPosition(cursorX, cursorY);

    ZCR_TextAxis* pNewActiveText = nullptr;
    for (auto& text : texts)
    {
        if (text.MakeCursorCollision(cursorX, cursorY, pNewActiveText ? pNewActiveText->depth : 1.f)) pNewActiveText = &text;
        if (pActiveAxisText && pNewActiveText == pActiveAxisText) return false;  //  cursor on active axis text, that is closest (depth) text
    }

    bool needRedraw = false;
    
    if (isButtonPressed && pActivePressedText)
    {
        if (pActivePressedText == pNewActiveText) return false;     //  haven't left pActivePressedText area
        pActivePressedText->SetState(ZCR_TextAxis::S_Default);      //  leave pActivePressedText area
        pActivePressedText = nullptr;
        needRedraw = true;
    }
    else if (pActiveMoveText)
    {
        if (pActiveMoveText == pNewActiveText) return false;    //  haven't left pActiveMoveText area
        pActiveMoveText->SetState(ZCR_TextAxis::S_Default);     //  leave pActiveMoveText area
        needRedraw = true;
    }
    pActiveMoveText = pNewActiveText;
    if (pActiveMoveText)
    {
        pActiveMoveText->SetState(ZCR_TextAxis::S_UnderCursor);     //  entered pActiveMoveText area
        needRedraw = true;
    }
    return needRedraw;      //  if false movement is in an empty area
}

bool ZCR_TextAxises::LeaveActiveArea()
{
    if (!pActiveMoveText) return false;
    
    pActiveMoveText->SetState(ZCR_TextAxis::S_Default);
    return true;
}

bool ZCR_TextAxises::MouseLeftButtonDown()
{
    isButtonPressed = true;
    if (pActiveMoveText)
    {
        pActiveMoveText->SetState(ZCR_TextAxis::S_Pressed);
        pActivePressedText = pActiveMoveText;
        pActiveMoveText = nullptr;
        return true;
    }
    return false;
}

void ZCR_TextAxises::MouseLeftButtonUp()
{
    isButtonPressed = false;
    if (pActivePressedText)
    {
        if (pActiveAxisText) pActiveAxisText->SetState(ZCR_TextAxis::S_Default);
        pActiveAxisText = pActivePressedText;
        pActivePressedText = nullptr;
        makeActiveAxisDefault = false;      //  next line will call ZCR_TextAxises::SetPosition() from camera, we don't need changing pActiveAxisText in that call
        pActiveAxisText->MouseLeftButtonUp();
    }
}

void ZCR_TextAxises::PrepareReconnect()
{
    if (pActiveAxisText) makeActiveAxisDefault = false;      //  if pActiveAxisText still not nullptr, don't changing it in next ZCR_TextAxises::SetPosition() call 
}

void ZCR_TextAxises::CamMoveWhileDeactivated()
{
    if (!pActiveAxisText) return;
    
    pActiveAxisText->SetState(ZCR_TextAxis::S_Default);
    pActiveAxisText = nullptr;
}