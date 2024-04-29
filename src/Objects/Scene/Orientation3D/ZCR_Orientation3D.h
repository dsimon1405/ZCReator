#pragma once

#include <ZC/Tools/ZC_OrthoSquare.h>
#include <ZC/Collision/ZC_MouseCollisionWindow.h>
#include <ZC/Events/ZC_SingleClickButton.h>
#include "ZCR_TextAxises.h"
#include <ZC/Objects/Camera/ZC_Camera.h>

class ZCR_Orientation3D : public ZC_MouseCollisionWindow
{
public:
    ZCR_Orientation3D();
    virtual ~ZCR_Orientation3D();

    void Activate();
    void Deactivate();

private:
    static inline const float textureSize = 100.f;

    ZC_Render* pRender;
    ZC_Camera camera;   //  camera only for that pRender
    ZCR_TextAxises textAxices;
    ZC_OrthoSquare orthoSquare;
    ZC_SConnection sconCamPosChange;
    bool needCalculateTextQuads = true;   //  calculating textAxices collision quads, only once, on first call VMouseMoveCollision() after camera rotetion
    ZC_SingleClickButton scbMouseLeft;
    bool isActive = true;

    ZC_Render* CreateRender();
    void SetPosition(const ZC_Vec3<float>& camPos);

    void VMouseMoveCollision(float time) override;
    void VMouseMoveCollisionEnd(float time) override;
    void CalculateTextPositions();
    void MouseLeftButtonDown(float time);
};