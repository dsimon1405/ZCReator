#pragma once

#include <ZC/Objects/Camera/ZC_Camera.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>
#include "ZCR_Axis.h"

class ZCR_Camera
{
public:
    ZCR_Camera();
    
    /*
    Trys to connect/disconnect camera's events, like start rotation or change position on mouse scroll. Can't do disconnection if camera allready rotate.

    Params:
    - needUse - true - to connect events, false - to try disconnect events.

    Return:
    true if camera already rotate, otherwise false. 
    */
    bool UseEvents(bool needUse);
    void SetAxis(ZCR_Axis axis);
    void SetLookOn(const ZC_Vec3<float>& lookOn);

private:
    ZC_Camera camera;

    const float speedMove = 10000.f;
    ZC_Vec3<float> dirFront,
        dirUp,
        dirRight;
    bool isDirsActual = false;

    bool isNormalHorizontalOrientation = true;
    const float maxDistanceToObject = 100.f,
        minDistanceToObject = 0.2f,
        sensivityScroll = 1.f;
    float distanceToObject = 15.f;
    
    const float sensitivityRotation = 1.f;
    float horizontalAngle = 0.f,
        verticalAngle = 0.f;
    bool isRotation = false,
        needUseEvents = true;

    ZC_SConnection sconButton_M_MIDLE;
    ZC_SConnection sconMouse;

    ZC_Camera CreateCamera();
    void Move(const ZC_Vec3<float>& step) noexcept;
    // void W(float time);
    // void S(float time);
    // void A(float time);
    // void D(float time);
    // void Q(float time);
    // void E(float time);
    void MouseMoveCallback(float x, float y, float xRel, float yRel, float time);
    void SetVerticalAngleMoreOrLessThan90(bool isMoreThan90);
    void RotateAroundObject();
    void CalculateDirections();
    void MouseWheelDown(float time);
    void MouseWheelUp(float time);
    void MouseWheelScroll(float rotationHorizontal, float rotationVertical, float time);

    ZC_Vec3<float> CalculateOrientation3DPosition();
};