#include "ZCR_Camera.h"

#include <ZC/Events/ZC_Events.h>
#include <ZC/Video/ZC_Window.h>

ZCR_Camera::ZCR_Camera()
    : camera(ZC_Camera::CreateCamera({ZC_Perspective(45.f, 0.01f, 100.f),
        ZC_View({0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, true)}, ZC_Ortho())),
    sconButton_M_MIDLE(ZC_Events::ConnectButtonDown(ZC_ButtonID::M_MIDLE, { &ZCR_Camera::ButtonMouseWheelDown, this })),
    sconMouse(ZC_Events::ConnectMouseScrollOnceInFrame({ &ZCR_Camera::MouseScroll, this }))
{
    MouseMoveAroundObject(0, 0, -120, 20, 0);

    // sConButtonMouseWheel = sConButtonMouseWheel;
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_W, ZC_Button::State::Down}, {&ZCR_Camera::W,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_S, ZC_Button::State::Down}, {&ZCR_Camera::S,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_A, ZC_Button::State::Down}, {&ZCR_Camera::A,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_D, ZC_Button::State::Down}, {&ZCR_Camera::D,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_Q, ZC_Button::State::Down}, {&ZCR_Camera::Q,this});
    // ZC_ButtonOperator::Connect({ZC_ButtonID::K_E, ZC_Button::State::Down}, {&ZCR_Camera::E,this});
    // ZC_Mouse::ConnectMove({&ZCR_Camera::MouseMoveAroundObject, this});
}

void ZCR_Camera::Move(const ZC_Vec3<float>& step) noexcept
{
    camera->SetLookOn(camera->GetLookOn() + step)
          .SetCamPos(camera->GetCamPos() + step);
}

void ZCR_Camera::W(float time) {Move(dirFront * time * speedMove);}

void ZCR_Camera::S(float time) {Move(dirFront * -1 * time * speedMove);}

void ZCR_Camera::A(float time) {Move(dirRight * -1 * time * speedMove);}

void ZCR_Camera::D(float time) {Move(dirRight * time * speedMove);}

void ZCR_Camera::Q(float time) {Move(dirUp * time * speedMove);}

void ZCR_Camera::E(float time) {Move(dirUp * -1 * time * speedMove);}

void ZCR_Camera::MouseMoveAroundObject(float x, float y, float xRel, float yRel, float time)
{
    // ZC_cout("x = " + std::to_string(x) + " = " + std::to_string((x / width) * 2 - 1.f) + "; "
    //     + "y = " + std::to_string(y) + " = " + std::to_string((1.f - (y / height)) * 2 - 1.f)
    //     + "     {" + std::to_string(p1[0]) + " " + std::to_string(p1[1]) + " " + std::to_string(p1[2]) + " " + std::to_string(p1[3]) + "}"
    //     + "     {" + std::to_string(p1[0] / p1[3]) + " " + std::to_string(p1[1] / p1[3]) + " " + std::to_string(p1[2] / p1[3]) + "}"
    //     // + "     {" + std::to_string(p2[0] / p2[3]) + " " + std::to_string(p2[1] / p2[3]) + " " + std::to_string(p2[2] / p2[3]) + "}"
    //     + "     {" + std::to_string(p3[0] / p3[3]) + " " + std::to_string(p3[1] / p3[3]) + " " + std::to_string(p3[2] / p3[3]) + "}");

    if (xRel == 0 && yRel == 0) return;
    horizontalAngle += yRel * sensitivityRotation;
    if (horizontalAngle >= 90.f) horizontalAngle = 89.9f;
    else if (horizontalAngle <= -90.f) horizontalAngle = -89.9f;
    verticalAngle += xRel * sensitivityRotation;
    if (verticalAngle >= 360.f) verticalAngle -= 360.f;
    else if (verticalAngle <= -360.f) verticalAngle += 360.f;

    // static bool isNormalHorizontalOrientation = true;
    // horizontalAngle += isNormalHorizontalOrientation ? yRel * sensitivityRotation : - (yRel * sensitivityRotation);
    // if (horizontalAngle == 90.f) horizontalAngle = 89.9f;
    // else if (horizontalAngle == -90.f) horizontalAngle = -89.9f;
    // else if (horizontalAngle > 90.f)
    // {
    //     horizontalAngle = 90.f - (horizontalAngle - 90.f);

    //     isNormalHorizontalOrientation = !isNormalHorizontalOrientation;
    //     isNormalHorizontalOrientation ? camera->SetUp({0.f, 0.f, 1.f}) : camera->SetUp({0.f, 0.f, -1.f});
    //     if (verticalAngle >= 0.f)
    //     {
    //         verticalAngle += 180.f;
    //         if (verticalAngle >= 360.f) verticalAngle -= 360.f;
    //     }
    //     else if (verticalAngle < 0.f)
    //     {
    //         verticalAngle -= 180.f;
    //         if (verticalAngle <= -360.f) verticalAngle += 360.f;
    //     }
    // }
    // else if (horizontalAngle <= -90.f)
    // {
    //     horizontalAngle = -90.f - (horizontalAngle + 90.f);
        
    //     isNormalHorizontalOrientation = !isNormalHorizontalOrientation;
    //     isNormalHorizontalOrientation ? camera->SetUp({0.f, 0.f, 1.f}) : camera->SetUp({0.f, 0.f, -1.f});
    //     if (verticalAngle >= 0.f)
    //     {
    //         verticalAngle += 180.f;
    //         if (verticalAngle >= 360.f) verticalAngle -= 360.f;
    //     }
    //     else if (verticalAngle < 0.f)
    //     {
    //         verticalAngle -= 180.f;
    //         if (verticalAngle <= -360.f) verticalAngle += 360.f;
    //     }
    // }
    // verticalAngle += isNormalHorizontalOrientation ? xRel * sensitivityRotation : - (xRel * sensitivityRotation);
    // if (verticalAngle >= 360.f) verticalAngle -= 360.f;
    // else if (verticalAngle <= -360.f) verticalAngle += 360.f;
    
    float camX = 0.f,
          camY = 0.f,
          camZ = 0.f;
    //  horizontal rotaion (rotate around horizont)
    float horRad = ZC_Vec::Radians(horizontalAngle);
    camY = distanceToObject * cos(horRad);
    camZ = distanceToObject * sin(horRad);
    //  vertical rotation (rotate around axis arthogonal to horizont)
    float vertRad = ZC_Vec::Radians(verticalAngle);
    camX = camY * cos(vertRad);
    camY = camY * sin(vertRad);
    
    auto lookOn = camera->GetLookOn();
    camera->SetCamPos({ camX + lookOn[0], camY + lookOn[1], camZ + lookOn[2] });

    // ZC_cout("angX = " + std::to_string(angleX) + "angY = " + std::to_string(angleY) + "x = " + std::to_string(camX) + "y = " + std::to_string(camY) + "z = " + std::to_string(camZ));
    isDirsActual = false;
}

void ZCR_Camera::CalculateDirections()
{
    dirFront = ZC_Vec::Normalize(camera->GetLookOn() - camera->GetCamPos());
    dirRight = ZC_Vec::Cross(dirFront, {0.f,0.f,1.f});    //  world up
    dirUp = ZC_Vec::Cross(dirRight, dirFront);
    isDirsActual = true;
}

void ZCR_Camera::ButtonMouseWheelDown(float time)
{
    sconButton_M_MIDLE.Disconnect();
    sconButton_M_MIDLE = ZC_Events::ConnectButtonUp(ZC_ButtonID::M_MIDLE, { &ZCR_Camera::ButtonMouseWheelUp, this });
    sconMouse.Disconnect(); //  disconnect scroll
    sconMouse = ZC_Events::ConnectMouseMoveOnceInFrame({ &ZCR_Camera::MouseMoveAroundObject, this });
    // ZC_Window::HideCursor();
}

void ZCR_Camera::ButtonMouseWheelUp(float time)
{
    sconButton_M_MIDLE.Disconnect();
    sconButton_M_MIDLE = ZC_Events::ConnectButtonDown(ZC_ButtonID::M_MIDLE, { &ZCR_Camera::ButtonMouseWheelDown, this });
    sconMouse.Disconnect(); //  disconnect mouse MouseMoveAroundObject
    sconMouse = ZC_Events::ConnectMouseScrollOnceInFrame({ &ZCR_Camera::MouseScroll, this });   //  return scroll connection
    // ZC_Window::ShowCursor();
}

void ZCR_Camera::MouseScroll(float rotationHorizontal, float rotationVertical, float time)
{
    if (rotationVertical == 0.f || (distanceToObject == minDistanceToObject && rotationVertical > 0.f) ||
        (distanceToObject == maxDistanceToObject && rotationVertical < 0.f)) return;
    if (!isDirsActual) CalculateDirections();
    float newDistance = distanceToObject - (rotationVertical * sensivityScroll);
    if (newDistance >= maxDistanceToObject)
    {
        distanceToObject = maxDistanceToObject;
        camera->SetCamPos(camera->GetLookOn() - (dirFront * maxDistanceToObject));
    }
    else if (newDistance <= minDistanceToObject)
    {
        distanceToObject = minDistanceToObject;
        camera->SetCamPos(camera->GetLookOn() - (dirFront * distanceToObject));
    }
    else
    {
        distanceToObject = newDistance;
        camera->SetCamPos(camera->GetLookOn() - (dirFront * distanceToObject));
    }
}