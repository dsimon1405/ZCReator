#include "ZCR_Camera.h"

#include <ZC/Events/ZC_Events.h>
#include <ZC/Video/ZC_Window.h>
#include "ZCR_Scene.h"

ZCR_Camera::ZCR_Camera()
    : camera(CreateCamera()),
    sconButton_M_MIDLE(ZC_Events::ConnectButtonClick(ZC_ButtonID::M_MIDLE, { &ZCR_Camera::MouseWheelDown, this }, { &ZCR_Camera::MouseWheelUp, this })),
    sconMouse(ZC_Events::ConnectMouseScrollOnceInFrame({ &ZCR_Camera::MouseWheelScroll, this }))
{
    camera.MakeActive();
    // MouseMoveAroundObject(0, 0, -120, 20, 0);

    // sConButtonMouseWheel = sConButtonMouseWheel;
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_W, ZC_Button::State::Down}, {&ZCR_Camera::W,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_S, ZC_Button::State::Down}, {&ZCR_Camera::S,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_A, ZC_Button::State::Down}, {&ZCR_Camera::A,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_D, ZC_Button::State::Down}, {&ZCR_Camera::D,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_Q, ZC_Button::State::Down}, {&ZCR_Camera::Q,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_E, ZC_Button::State::Down}, {&ZCR_Camera::E,this});
    // ZC_Mouse::ConnectMove({&ZCR_Camera::MouseMoveAroundObject, this});

    MouseMoveCallback(0, 0, 45, -45, 0);
    SetCameraLookOn({3.f, 0.f, 2.f});
}

void ZCR_Camera::SetCameraLookOn(const ZC_Vec3<float>& lookOn)
{
    auto curLookOn = camera.GetLookOn();
    if (curLookOn == lookOn) return;
    camera.SetPosition(lookOn + (camera.GetPosition() - curLookOn));
    camera.SetLookOn(lookOn);
}

bool ZCR_Camera::IsRotating()
{
    return isRotating;
}

void ZCR_Camera::SetCameraAxis(ZCR_Axis axis)
{
    switch (axis)
    {
    case ZCR_A_PositiveX: horizontalAngle = -90.f; verticalAngle = 0.f; break;
    case ZCR_A_NegativeX: horizontalAngle = 90.f; verticalAngle = 0.f; break;
    case ZCR_A_PositiveY: horizontalAngle = 180.f; verticalAngle = 0.f; break;
    case ZCR_A_NegativeY: horizontalAngle = 0.f; verticalAngle = 0.f; break;
    case ZCR_A_PositiveZ:
    {
        horizontalAngle = horizontalAngle >= 0.f ?
            horizontalAngle < 45.f ? 0.f : horizontalAngle < 135.f ? 90.f : horizontalAngle < 225.f ? 180.f : horizontalAngle < 315.f ? 270.f : 0.f 
            : horizontalAngle > -45.f ? 0.f : horizontalAngle > -135.f ? -90.f : horizontalAngle > -225.f ? -180.f : horizontalAngle > -315.f ? -270.f : 0.f;
        verticalAngle = 89.9f;
     } break;
    case ZCR_A_NegativeZ:
    {
        horizontalAngle = horizontalAngle >= 0.f ?
            horizontalAngle < 45.f ? 0.f : horizontalAngle < 135.f ? 90.f : horizontalAngle < 225.f ? 180.f : horizontalAngle < 315.f ? 270.f : 0.f 
            : horizontalAngle > -45.f ? 0.f : horizontalAngle > -135.f ? -90.f : horizontalAngle > -225.f ? -180.f : horizontalAngle > -315.f ? -270.f : 0.f;
        verticalAngle = -89.9f;
    } break;
    case ZCR_A_Free: return;
    }
    
    RotateAroundObject();
}

ZC_Camera ZCR_Camera::CreateCamera()
{
    int width, height;
    ZC_Window::GetSize(width, height);
    return ZC_Camera({0.f, -15, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, ZC_Perspective(45.f, static_cast<float>(width) / static_cast<float>(height), 0.01f, 100.f),
        ZC_Ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height)), true);
}

void ZCR_Camera::Move(const ZC_Vec3<float>& step) noexcept
{
    camera.SetLookOn(camera.GetLookOn() + step);
    camera.SetPosition(camera.GetPosition() + step);
}

// void ZCR_Camera::W(float time) {Move(dirFront * time * speedMove);}

// void ZCR_Camera::S(float time) {Move(dirFront * -1 * time * speedMove);}

// void ZCR_Camera::A(float time) {Move(dirRight * -1 * time * speedMove);}

// void ZCR_Camera::D(float time) {Move(dirRight * time * speedMove);}

// void ZCR_Camera::Q(float time) {Move(dirUp * time * speedMove);}

// void ZCR_Camera::E(float time) {Move(dirUp * -1 * time * speedMove);}


void ZCR_Camera::MouseMoveCallback(float x, float y, float xRel, float yRel, float time)
{
    // if (xRel == 0 && yRel == 0) return;
    // horizontalAngle += yRel * sensitivityRotation;
    // if (horizontalAngle >= 90.f) horizontalAngle = 89.9f;
    // else if (horizontalAngle <= -90.f) horizontalAngle = -89.9f;
    // verticalAngle += xRel * sensitivityRotation;
    // if (verticalAngle >= 360.f) verticalAngle -= 360.f;
    // else if (verticalAngle <= -360.f) verticalAngle += 360.f;

    verticalAngle += sensitivityRotation * (isNormalHorizontalOrientation ? - yRel : yRel);
    if (verticalAngle == 90.f) verticalAngle = 89.9f;
    else if (verticalAngle == -90.f) verticalAngle = -89.9f;
    else if (verticalAngle > 90.f) SetVerticalAngleMoreOrLessThan90(true);
    else if (verticalAngle < -90.f) SetVerticalAngleMoreOrLessThan90(false);
    horizontalAngle += sensitivityRotation * (isNormalHorizontalOrientation ? xRel : - xRel);
    if (horizontalAngle >= 360.f) horizontalAngle -= 360.f;
    else if (horizontalAngle <= -360.f) horizontalAngle += 360.f;

    RotateAroundObject();

    ZCR_Scene::GetScene()->SetAxise(ZCR_A_Free);
}

void ZCR_Camera::SetVerticalAngleMoreOrLessThan90(bool isMoreThan90)
{
    verticalAngle = isMoreThan90 ? 90.f - (verticalAngle - 90.f) : -90.f - (verticalAngle + 90.f);

    isNormalHorizontalOrientation = !isNormalHorizontalOrientation;
    isNormalHorizontalOrientation ? camera.SetUp({0.f, 0.f, 1.f}) : camera.SetUp({0.f, 0.f, -1.f});
    horizontalAngle += horizontalAngle >= 0.f ? 180.f : -180.f;
}

void ZCR_Camera::RotateAroundObject()
{
    // //  manual rotation variant
    // float camX = 0.f,
    //       camY = 0.f,
    //       camZ = 0.f;
    // //  vertical rotaion (rotate around horizont)
    // float verticalRad = ZC_Vec::Radians(verticalAngle);
    // camX = distanceToObject * cos(verticalRad);
    // camZ = distanceToObject * sin(verticalRad);
    // //  horizontal rotation (rotate around world up vector)
    // float horizontalRad = ZC_Vec::Radians(horizontalAngle);
    // camY = camX * sin(horizontalRad);
    // camX = camX * cos(horizontalRad);

    // auto lookOn = camera.GetLookOn();
    // camera.SetPosition({ lookOn[0] + camX, lookOn[1] - camY, lookOn[2] + camZ });    //  on start positive X, positive Y, positive Z, if positive camX, camY, camZ

    //  analogue with rotation of the model matrix!
    ZC_Mat4<float> newModel(1.f);
    newModel.Rotate(horizontalAngle, { 0.f, 0.f, -1.f }).Rotate(verticalAngle, { -1.f, 0.f, 0.f });
    ZC_Vec4<float> camPos = newModel * ZC_Vec4<float>(0.f, -distanceToObject, 0.f, 1.f);    //  start position on -y for capatability with ZC_TurnToCamera
    auto lookOn = camera.GetLookOn();
    camera.SetPosition({ lookOn[0] + camPos[0], lookOn[1] + camPos[1], lookOn[2] + camPos[2] });

    isDirsActual = false;
}

void ZCR_Camera::CalculateDirections()
{
    if (isDirsActual) return;
    dirFront = ZC_Vec::Normalize(camera.GetLookOn() - camera.GetPosition());
    dirRight = ZC_Vec::Normalize(ZC_Vec::Cross(dirFront, {0.f,0.f,1.f}));    //  world up
    dirUp = ZC_Vec::Normalize(ZC_Vec::Cross(dirRight, dirFront));
    isDirsActual = true;
}

void ZCR_Camera::MouseWheelDown(ZC_ButtonID buttonId, float time)
{
    isRotating = true;
    sconMouse.NewConnection(ZC_Events::ConnectMouseMoveOnceInFrame({ &ZCR_Camera::MouseMoveCallback, this }));      //  disconnect scroll, connect mouse move callback
}

void ZCR_Camera::MouseWheelUp(ZC_ButtonID buttonId, float time)
{
    isRotating = false;
    sconMouse.NewConnection(ZC_Events::ConnectMouseScrollOnceInFrame({ &ZCR_Camera::MouseWheelScroll, this }));      //  disconnect mouse move callback, connect scroll
}

void ZCR_Camera::MouseWheelScroll(float rotationHorizontal, float rotationVertical, float time)
{
    if (rotationVertical == 0.f || (distanceToObject == minDistanceToObject && rotationVertical > 0.f) ||
        (distanceToObject == maxDistanceToObject && rotationVertical < 0.f)) return;
    CalculateDirections();
    float newDistance = distanceToObject - (rotationVertical * sensivityScroll);
    if (newDistance >= maxDistanceToObject)
    {
        distanceToObject = maxDistanceToObject;
        camera.SetPosition(camera.GetLookOn() - (dirFront * maxDistanceToObject));
    }
    else if (newDistance <= minDistanceToObject)
    {
        distanceToObject = minDistanceToObject;
        camera.SetPosition(camera.GetLookOn() - (dirFront * distanceToObject));
    }
    else
    {
        distanceToObject = newDistance;
        camera.SetPosition(camera.GetLookOn() - (dirFront * distanceToObject));
    }
}