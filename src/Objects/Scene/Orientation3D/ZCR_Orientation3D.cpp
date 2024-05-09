#include "ZCR_Orientation3D.h"

#include <Renderer/ZCR_RenderLevel.h>
#include <Objects/Scene/ZCR_Scene.h>
#include <ZC/Events/ZC_Events.h>

#include <cassert>

ZCR_Orientation3D::ZCR_Orientation3D()
    : ZC_MouseCollisionWindow(ZC_MouseCollisionWindow::E_Move, &textureSize, &textureSize, &textureSize, &textureSize, true),
    pRender(ZC_Renders::CreateRender(ZCR_RL_Orientation3D, ZC_Render::DS_None,
        ZC_FBO(ZC_Viewport(0, 0, textureSize, textureSize), 0, ZC_FBO::CIF_GL_RGBA8, ZC_FBO::DSIF_GL_DEPTH_COMPONENT24, ZC_FBO::RT_Color, false))),
    camera(ZC_Camera::GetActiveCamera()->GetPosition(), ZC_Camera::GetActiveCamera()->GetLookOn(), ZC_Camera::GetActiveCamera()->GetUp(),
        ZC_Perspective(45.f, textureSize / textureSize, 0.01f, 100.f), ZC_Ortho(0.f, textureSize, 0.f, textureSize), false, ZCR_RL_Orientation3D),
    orthoSquare(pRender->GetColorTexture(), textureSize, textureSize, 3.f, 3.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel, ZC_RL_Default, true, {}),
    ecCamPosChange(ZC_Camera::GetActiveCamera()->ConnectChangeCameraPosition({ &ZCR_Orientation3D::SetPosition, this })),
    ecWindowResize(ZC_Events::ConnectWindowResize({ &ZCR_Orientation3D::WindowResize, this }))
{
    const ZC_Vec2<float>& squareCoords = orthoSquare.GetIndents();
    this->ReinstallCoords(&squareCoords[0], &squareCoords[1], &textureSize, &textureSize);
    this->UseManualMove(true);

    pRender->SetClearColor(0.f, 0.f, 0.f, 0.f);

    SetPosition(ZC_Camera::GetActiveCamera()->GetPosition());
}

ZCR_Orientation3D::~ZCR_Orientation3D()
{
    ecCamPosChange.Disconnect();
    ecMouseLeftButtonClick.Disconnect();
}

void ZCR_Orientation3D::Activate()
{
    if (isActive) return;
    isActive = true;
    ecMouseLeftButtonClick.NewConnection(ZC_Events::ConnectButtonClick(ZC_ButtonID::M_LEFT, { &ZCR_Orientation3D::MouseLeftButtonDown, this },
        { &ZCR_Orientation3D::MouseLeftButtonUp, this }));
    orthoSquare.NeedDraw(true);
    textAxices.PrepareReconnect();
    SetPosition(ZC_Camera::GetActiveCamera()->GetPosition());
    this->ConnectMCW();
}

void ZCR_Orientation3D::Deactivate()
{
    if (!isActive) return;
    isActive = false;
    ecMouseLeftButtonClick.Disconnect();
    orthoSquare.NeedDraw(false);
    this->DisconnectMCW();
}

bool ZCR_Orientation3D::IsActive() const noexcept
{
    return isActive;
}

void ZCR_Orientation3D::SetPosition(const ZC_Vec3<float>& camPos)
{
    if (isActive)
    {
        auto lookOn = ZC_Camera::GetActiveCamera()->GetLookOn();
        
        camera.SetPosition(camPos);
        camera.SetLookOn(lookOn);
        camera.SetUp(ZC_Camera::GetActiveCamera()->GetUp());

        textAxices.SetPosition(ZC_Vec::MoveByLength(camPos, lookOn - camPos, 3.3f));        //  3.3 -> distance from camera to loockOn (orientation3D)

        pRender->SetDrawState(ZC_Render::DS_OneFrame);

        needCalculateTextQuads = true;
    }
    else textAxices.CamMoveWhileDeactivated();
}

void ZCR_Orientation3D::VCrusorMoveCollision(float time)
{
    if (ZCR_Scene::GetScene()->IsRotating()) return;  // camera rotates, can't start cursor collision event before stop rotation

    if (needCalculateTextQuads)
    {
        textAxices.CalculateQuads(*camera.GetPerspectiveView(), orthoSquare.GetIndents(), textureSize, textureSize);
        needCalculateTextQuads = false;
    }

    if (textAxices.MakeCursorMoveCollision()) pRender->SetDrawState(ZC_Render::DS_OneFrame);

    if (!ecMouseLeftButtonClick.IsConnected())
        ecMouseLeftButtonClick.NewConnection(ZC_Events::ConnectButtonClick(ZC_ButtonID::M_LEFT, { &ZCR_Orientation3D::MouseLeftButtonDown, this },
            { &ZCR_Orientation3D::MouseLeftButtonUp, this }));
}

void ZCR_Orientation3D::VCursorMoveCollisionEnd(float time)
{
    if (textAxices.LeaveActiveArea()) pRender->SetDrawState(ZC_Render::DS_OneFrame);
    ecMouseLeftButtonClick.Disconnect();
}

void ZCR_Orientation3D::MouseLeftButtonDown(ZC_ButtonID buttonId, float time)
{
    if (textAxices.MouseLeftButtonDown()) pRender->SetDrawState(ZC_Render::DS_OneFrame);
}

void ZCR_Orientation3D::MouseLeftButtonUp(ZC_ButtonID buttonId, float time)
{
    textAxices.MouseLeftButtonUp();
}

void ZCR_Orientation3D::WindowResize(float widht, float height)
{
    needCalculateTextQuads = true;
}