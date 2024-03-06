#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RS.h>
// #include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>

struct ZCR_Orientation3D
{
    ZC_uptr<ZC_RS> upRSLine;
    ZC_uptr<ZC_RendererSetAndDrawingSet> upRASDLine;
    ZC_Mat4<float> matModel { 1.f };
    
    const float sensitivityRotation = 1.f;
    float verticalAngle = 0.f,
        horizontalAngle = 0.f;

    ZC_SConnection sconButton_M_MIDLE;
    ZC_SConnection sconMouse;

    ZCR_Orientation3D(float xRel, float yRel);
    ZC_uptr<ZC_RS> MakeRS();
    void MouseMoveAroundObject(float x, float y, float xRel, float yRel, float time);
    void ButtonMouseWheelDown(float time);
    void ButtonMouseWheelUp(float time);
};

#include <ZC/Video/OpenGL/Renderer/ZC_RSNonTex.h>
#include <ZC/Tools/Math/ZC_Limits.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC/Tools/Math/ZC_Mat.h>

ZCR_Orientation3D::ZCR_Orientation3D(float xRel, float yRel)
    : upRSLine(MakeRS()),
    upRASDLine(upRSLine->Make_uptrRendererSetDrawingSet(nullptr, 0, 0)),
    sconButton_M_MIDLE(ZC_Events::ConnectButtonDown(ZC_ButtonID::M_MIDLE, { &ZCR_Orientation3D::ButtonMouseWheelDown, this }))
{
    upRASDLine->SwitchToLvl(ZC_RendererSet::Level::Drawing);

    MouseMoveAroundObject(0, 0, xRel, yRel, 0);
}

ZC_uptr<ZC_RS> ZCR_Orientation3D::MakeRS()
{
    uchar colors[]
    {
        255, 0, 0,
        0, 255, 0,
        0, 0, 255,
    };

    ZC_Buffer vbo(GL_ARRAY_BUFFER);
    const size_t threeVerticesByteSize = sizeof(ZC_Vec4<float>) * 3;
    vbo.BufferData(threeVerticesByteSize + sizeof(colors), nullptr, GL_DYNAMIC_DRAW);
    vbo.BufferSubData(threeVerticesByteSize, sizeof(colors), colors);
    
    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawArrays>(GL_POINTS, 0, 3);   //  draw as GL_POINTS but drawing 3 lines in geometry shader

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::ZCR_LineOrientation3D);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConData, vbo, nullptr, 0, 3);   //  last param = 3, vertices amount

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));

    return { new ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
}

void ZCR_Orientation3D::MouseMoveAroundObject(float x, float y, float xRel, float yRel, float time)
{
    if (xRel == 0 && yRel == 0) return;
    horizontalAngle -= yRel * sensitivityRotation;
    if (horizontalAngle >= 90.f) horizontalAngle = 89.9f;
    else if (horizontalAngle <= -90.f) horizontalAngle = -89.9f;
    verticalAngle -= xRel * sensitivityRotation;
    if (verticalAngle >= 360.f) verticalAngle -= 360.f;
    else if (verticalAngle <= -360.f) verticalAngle += 360.f;
                                                                                                                //  add chek on world up, if full round horizontal rotation !!!! 
    static const auto perspectiveView = ZC_Mat::Perspective(45.f, 800.f/600.f, 0.1f, 100.f) * ZC_Mat::LookAt(ZC_Vec3<float>{5.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f});
    static const float linesLength = 1.f;

    float positiveX_Y = 0.f,
        positiveX_X = 0.f,
        positiveX_Z = 0.f;
    //  horizontal rotaion (rotate around horizont)
    float horRad = ZC_Vec::Radians(horizontalAngle);
    positiveX_X = linesLength * cos(horRad);
    positiveX_Z = linesLength * sin(horRad);
    //  vertical rotation (rotate around axis arthogonal to horizont)
    float vertRad = ZC_Vec::Radians(verticalAngle);
    positiveX_Y = positiveX_X * sin(vertRad);
    positiveX_X = positiveX_X * cos(vertRad);

    ZC_Vec3<float> positiveX = ZC_Vec::Normalize(ZC_Vec3<float>(positiveX_X, positiveX_Y, positiveX_Z)),
        positiveY = ZC_Vec::Normalize(ZC_Vec::Cross({0.f, 0.f, 1.f}, positiveX)), //  add chek on world up, if full round horizontal rotation !!!!
        positiveZ = ZC_Vec::Normalize(ZC_Vec::Cross(positiveX, positiveY));
    // positiveY = ZC_Vec::Cross(positiveZ, positiveX);
    
    auto normP_X = ZC_Vec::Normalize(positiveX),    //  check !!!!!!!
        mormP_Y = ZC_Vec::Normalize(positiveY),
        normP_Z = ZC_Vec::Normalize(positiveZ);

    ZC_Vec4<float> positivePointsOfLines[]
    {
        perspectiveView * ZC_Vec4<float>(positiveX, 1.f),
        perspectiveView * ZC_Vec4<float>(positiveY, 1.f),
        perspectiveView * ZC_Vec4<float>(positiveZ, 1.f)
    };

    const size_t threeVerticesByteSize = sizeof(ZC_Vec4<float>) * 3;
    upRSLine->buffers.begin()->BufferSubData(0, threeVerticesByteSize, positivePointsOfLines);
}

void ZCR_Orientation3D::ButtonMouseWheelDown(float time)
{
    sconButton_M_MIDLE.Disconnect();
    sconButton_M_MIDLE = ZC_Events::ConnectButtonUp(ZC_ButtonID::M_MIDLE, { &ZCR_Orientation3D::ButtonMouseWheelUp, this });
    sconMouse = ZC_Events::ConnectMouseMoveOnceInFrame({ &ZCR_Orientation3D::MouseMoveAroundObject, this });
    // ZC_Window::HideCursor();
}

void ZCR_Orientation3D::ButtonMouseWheelUp(float time)
{
    sconButton_M_MIDLE.Disconnect();
    sconButton_M_MIDLE = ZC_Events::ConnectButtonDown(ZC_ButtonID::M_MIDLE, { &ZCR_Orientation3D::ButtonMouseWheelDown, this });
    sconMouse.Disconnect(); //  disconnect mouse MouseMoveAroundObject
    // ZC_Window::ShowCursor();
}