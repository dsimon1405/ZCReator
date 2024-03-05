#pragma once

#include <ZC/Objects/Camera/ZC_Camera.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>

class ZCR_Camera
{
public:
    ZCR_Camera();

private:
    ZC_upCamera camera;

    const float speedMove = 10000.f;
    ZC_Vec3<float> dirFront,
        dirUp,
        dirRight;
    bool isDirsActual = false;

    const float maxDistanceToObject = 100.f,
        minDistanceToObject = 0.2f,
        sensivityScroll = 1.f;
    float distanceToObject = 15.f;
        
    const float sensitivityRotation = 1.f;
    float verticalAngle = 0.f,
        horizontalAngle = 0.f;

    ZC_SConnection sconButton_M_MIDLE;
    ZC_SConnection sconMouse;

    void Move(const ZC_Vec3<float>& step) noexcept;
    void W(float time);
    void S(float time);
    void A(float time);
    void D(float time);
    void Q(float time);
    void E(float time);
    void MouseMoveAroundObject(float x, float y, float xRel, float yRel, float time);
    void CalculateDirections();
    void ButtonMouseWheelDown(float time);
    void ButtonMouseWheelUp(float time);
    void MouseScroll(float rotationHorizontal, float rotationVertical, float time);
};