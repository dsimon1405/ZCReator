#include "ZCR_TextAxises.h"

#include <ZC/Video/ZC_Window.h>

void ZCR_TextAxises::SetPosition(const ZC_Vec3<float>& newPosition)
{
    if (pActiveAxisText)
    {
        if (!fistCamMoveAfterMakeAxisActive)
        {
            pActiveAxisText->SetState(ZCR_TextAxis::S_Default);
            pActiveAxisText = nullptr;
        }
        fistCamMoveAfterMakeAxisActive = false;
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
    ZC_Window::GetCursorPosition(cursorX, cursorY);

    ZCR_TextAxis* pNewActiveText = nullptr;
    for (auto& text : texts)
    {
        if (text.MakeCursorCollision(cursorX, cursorY, pNewActiveText ? pNewActiveText->depth : 1.f)) pNewActiveText = &text;
        if (pActiveAxisText && pNewActiveText == pActiveAxisText) return false;  //  cursor on active axis text, that is closest (depth) text
    }
    
    if (pNewActiveText == pActiveMoveText) return false;

    if (pActiveMoveText) pActiveMoveText->SetState(ZCR_TextAxis::S_Default);
    pActiveMoveText = pNewActiveText;
    if (pActiveMoveText) pActiveMoveText->SetState(ZCR_TextAxis::S_UnderCursor);
    return true;
}

bool ZCR_TextAxises::LeaveActiveArea()
{
    if (!pActiveMoveText) return false;
    
    pActiveMoveText->SetState(ZCR_TextAxis::S_Default);
    return true;
}

void ZCR_TextAxises::MouseLeftButtonDown()
{
    if (pActiveMoveText)
    {
        if (pActiveAxisText) pActiveAxisText->SetState(ZCR_TextAxis::S_Default);
        pActiveAxisText = pActiveMoveText;
        pActiveMoveText = nullptr;
        fistCamMoveAfterMakeAxisActive = true;
        pActiveAxisText->MouseLeftButtonDown();
    }
}