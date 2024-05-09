#pragma once

#include <ZC/Tools/ZC_OrthoSquare.h>
#include <ZC/Collision/ZC_MouseCollisionWindow.h>
#include "ZCR_TextAxises.h"
#include <ZC/Objects/Camera/ZC_Camera.h>
#include <ZC/Events/ZC_ButtonID.h>

class ZCR_Orientation3D : public ZC_MouseCollisionWindow
{
public:
    ZCR_Orientation3D();
    virtual ~ZCR_Orientation3D();

    void Activate();
    void Deactivate();
    bool IsActive() const noexcept;

private:
    static inline const float textureSize = 100.f;

    ZC_Render* pRender;
    ZC_Camera camera;   //  camera only for that pRender
    ZCR_TextAxises textAxices;
    ZC_OrthoSquare orthoSquare;
    ZC_EC ecCamPosChange,
        ecMouseLeftButtonClick,
        ecWindowResize;
    bool needCalculateTextQuads = true;   //  calculating textAxices collision quads, only once, on first call VCrusorMoveCollision() after camera rotetion
    bool isActive = true;

    ZC_Render* CreateRender();
    void SetPosition(const ZC_Vec3<float>& camPos);

    void VCrusorMoveCollision(float time) override;
    void VCursorMoveCollisionEnd(float time) override;
    void CalculateTextPositions();
    void MouseLeftButtonDown(ZC_ButtonID buttonId, float time);
    void MouseLeftButtonUp(ZC_ButtonID buttonId, float time);
    void WindowResize(float widht, float height);
};