#include "ZCR_Orientation3D.h"

#include <Renderer/ZCR_FrameBuffer.h>

#include <cassert>

ZCR_Orientation3D::ZCR_Orientation3D(ZCR_Camera* _pZCR_Camera)
    : ZC_MouseCollisionWindow(ZC_MouseCollisionWindow::E_Move | ZC_MouseCollisionWindow::E_Down_Mouse_Left,
        &textureSize, &textureSize, &textureSize, &textureSize, true),
    pRender(ZC_Renders::CreateRender(ZCR_FB_Orientation3D, ZC_Render::DS_None,
        ZC_FBO(ZC_Viewport(0, 0, textureSize, textureSize), 0, ZC_FBO::CIF_GL_RGBA8, ZC_FBO::DSIF_GL_DEPTH_COMPONENT24, ZC_FBO::RT_Color, false))),
    camera(ZC_Camera::GetActiveCamera()->GetPosition(), ZC_Camera::GetActiveCamera()->GetLookOn(), ZC_Camera::GetActiveCamera()->GetUp(),
        ZC_Perspective(45.f, textureSize / textureSize, 0.01f, 100.f), ZC_Ortho(0.f, textureSize, 0.f, textureSize), false, ZCR_FB_Orientation3D),
    orthoSquare(pRender->GetColorTexture(), textureSize, textureSize, 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel, ZC_FB_Default, true, {}),
    sconCamPosChange(ZC_Camera::GetActiveCamera()->ConnectChangeCameraPosition({ &ZCR_Orientation3D::SetPosition, this })),
    pZCR_Camera(_pZCR_Camera),
    scbMouseLeft(ZC_ButtonID::M_LEFT, { &ZCR_Orientation3D::MouseLeftButtonDown, this }, nullptr)
{
    const ZC_Vec2<float>& squareCoords = orthoSquare.GetIndents();
    this->ReinstallCoords(&squareCoords[0], &squareCoords[1], &textureSize, &textureSize);
    this->UseManualMove(true);

    pRender->SetClearColor(0.f, 0.f, 0.f, 0.f);

    SetPosition(ZC_Camera::GetActiveCamera()->GetPosition());
}

void ZCR_Orientation3D::SetPosition(const ZC_Vec3<float>& camPos)
{
    auto lookOn = ZC_Camera::GetActiveCamera()->GetLookOn();
    
    camera.SetPosition(camPos);
    camera.SetLookOn(lookOn);
    camera.SetUp(ZC_Camera::GetActiveCamera()->GetUp());

    textAxices.SetPosition(ZC_Vec::MoveByLength(camPos, lookOn - camPos, 3.3f));        //  3.3 -> distance from camera to loockOn (orientation3D)

    pRender->SetDrawState(ZC_Render::DS_OneFrame);

    needCalculateTextQuads = true;
}

void ZCR_Orientation3D::VMouseMoveCollision(float time)
{
    if (pZCR_Camera->UseEvents(false)) return;  // camera rotates, can't start cursor collision event before stopping

    if (needCalculateTextQuads)
    {
        textAxices.CalculateQuads(*camera.GetPerspectiveView(), orthoSquare.GetIndents(), textureSize, textureSize);
        needCalculateTextQuads = false;
    }

    if (textAxices.MakeCursorMoveCollision()) pRender->SetDrawState(ZC_Render::DS_OneFrame);

    scbMouseLeft.Connect();
}

void ZCR_Orientation3D::VMouseMoveCollisionEnd(float time)
{
    pZCR_Camera->UseEvents(true);
    textAxices.LeaveActiveArea();
    scbMouseLeft.Disconnect();
}

void ZCR_Orientation3D::MouseLeftButtonDown(float time)
{
    textAxices.MouseLeftButtonDown();
}