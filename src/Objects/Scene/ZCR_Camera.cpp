#include "ZCR_Camera.h"

#include <ZC/Events/ZC_Events.h>
#include <ZC/Video/ZC_Window.h>

ZCR_Camera::ZCR_Camera()
    : camera(ZC_Camera::CreateCamera({ZC_Perspective(45.f, 0.1f, 100.f),
        ZC_View({0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, true)}, ZC_Ortho())),
    sconButton(ZC_Events::ConnectButtonDown(ZC_ButtonID::M_MIDLE, { &ZCR_Camera::ButtonMouseWheelDown, this }))
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

void ZCR_Camera::W(float time) {Move(front * time * speedMove);}

void ZCR_Camera::S(float time) {Move(front * -1 * time * speedMove);}

void ZCR_Camera::A(float time) {Move(right * -1 * time * speedMove);}

void ZCR_Camera::D(float time) {Move(right * time * speedMove);}

void ZCR_Camera::Q(float time) {Move(up * time * speedMove);}

void ZCR_Camera::E(float time) {Move(up * -1 * time * speedMove);}

void ZCR_Camera::MouseMoveAroundObject(float x, float y, float xRel, float yRel, float time)
{
    // ZC_cout("x = " + std::to_string(x) + " = " + std::to_string((x / width) * 2 - 1.f) + "; "
    //     + "y = " + std::to_string(y) + " = " + std::to_string((1.f - (y / height)) * 2 - 1.f)
    //     + "     {" + std::to_string(p1[0]) + " " + std::to_string(p1[1]) + " " + std::to_string(p1[2]) + " " + std::to_string(p1[3]) + "}"
    //     + "     {" + std::to_string(p1[0] / p1[3]) + " " + std::to_string(p1[1] / p1[3]) + " " + std::to_string(p1[2] / p1[3]) + "}"
    //     // + "     {" + std::to_string(p2[0] / p2[3]) + " " + std::to_string(p2[1] / p2[3]) + " " + std::to_string(p2[2] / p2[3]) + "}"
    //     + "     {" + std::to_string(p3[0] / p3[3]) + " " + std::to_string(p3[1] / p3[3]) + " " + std::to_string(p3[2] / p3[3]) + "}");
    
    if (xRel == 0 && yRel == 0) return;
    verticalAngle += xRel * sensitivity;
    if (verticalAngle >= 360.f) verticalAngle -= 360.f;
    if (verticalAngle <= -360.f) verticalAngle += 360.f;
    
    horizontalAngle += yRel * sensitivity;
    if (horizontalAngle >= 90.f) horizontalAngle = 89.f;
    if (horizontalAngle <= -90.f) horizontalAngle = -89.f;
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
    CalculateDirections();  //  ???
}

void ZCR_Camera::CalculateDirections()
{
    front = ZC_Vec::Normalize(camera->GetLookOn() - camera->GetCamPos());
    right = ZC_Vec::Cross(front, {0.f,0.f,1.f});    //  world up
    up = ZC_Vec::Cross(right, front);
}

void ZCR_Camera::ButtonMouseWheelDown(float time)
{
    sconButton.Disconnect();
    sconButton = ZC_Events::ConnectButtonUp(ZC_ButtonID::M_MIDLE, { &ZCR_Camera::ButtonMouseWheelUp, this });
    sconMouseMove = ZC_Events::ConnectMouseMove({ &ZCR_Camera::MouseMoveAroundObject, this });
        // ZC_Window::HideCursor();
}

void ZCR_Camera::ButtonMouseWheelUp(float time)
{
    sconButton.Disconnect();
    sconButton = ZC_Events::ConnectButtonDown(ZC_ButtonID::M_MIDLE, { &ZCR_Camera::ButtonMouseWheelDown, this });
    sconMouseMove.Disconnect();
    // ZC_Window::ShowCursor();
}