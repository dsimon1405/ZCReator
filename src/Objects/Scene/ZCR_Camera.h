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
    ZC_Vec3<float> front;
    ZC_Vec3<float> up;
    ZC_Vec3<float> right;

    const float maxDistanceToObject = 100.f;
    float distanceToObject = 15.f;
    float verticalAngle = 0.f;
    float horizontalAngle = 0.f;
    float sensitivity = 1.f;

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
    void MouseScroll(float rotationVertical, float rotationHorizontal, float time);
};