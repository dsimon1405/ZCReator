#pragma once

#include <ZC/Tools/ZC_OrthoSquare.h>
#include <Objects/Scene/ZCR_Camera.h>
#include <ZC/Collision/ZC_MouseCollisionWindow.h>
#include <ZC/Events/ZC_SingleClickButton.h>
#include "ZCR_TextAxises.h"

class ZCR_Orientation3D : public ZC_MouseCollisionWindow
{
public:
    ZCR_Orientation3D(ZCR_Camera* _pZCR_Camera);

private:
    static inline const float textureSize = 100.f;

    ZC_Render* pRender;
    ZC_Camera camera;   //  camera only for that pRender
    ZCR_TextAxises textAxices;
    ZC_OrthoSquare orthoSquare;
    ZC_SConnection sconCamPosChange;
    ZCR_Camera* pZCR_Camera;
    bool needCalculateTextQuads = true;   //  calculating textAxices collision quads, only once, on first call VMouseMoveCollision() after camera rotetion
    ZC_SingleClickButton scbMouseLeft;

    ZC_Render* CreateRender();
    void SetPosition(const ZC_Vec3<float>& camPos);

    void VMouseMoveCollision(float time) override;
    void VMouseMoveCollisionEnd(float time) override;
    void CalculateTextPositions();
    void MouseLeftButtonDown(float time);
};