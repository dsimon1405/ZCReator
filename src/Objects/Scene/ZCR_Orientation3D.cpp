#include "ZCR_Orientation3D.h"

#include <ZC/Video/OpenGL/Renderer/ZC_RSNonTex.h>
#include <ZC/Tools/Math/ZC_Limits.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC/Tools/Math/ZC_Mat.h>
#include "ZCR_Camera.h"

ZCR_Orientation3D::ZCR_Orientation3D(float windowAspect)
    : upRSLine(MakeRSLine()),
    upRASDLine(upRSLine->Make_uptrRendererSetDrawingSet(nullptr, 0, 0)),
    perspective(ZC_Mat::Perspective(45.f, windowAspect, 0.1f, 5.f)),
    view(ZC_Mat::LookAt(ZC_Vec3<float>{5.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f})),
    perspView(perspective * view)
{
    ZC_Events::ConnectWindowResize({ &ZCR_Orientation3D::WindowResizeCallback, this });
    upRASDLine->SwitchToLvl(ZC_RendererSet::Level::Drawing);
}

void ZCR_Orientation3D::MoveAroundObject(float _horizontalAngle, float _verticalAngle, bool _isNormalHorizontalOrientation)
{
    ZC_Mat4<float> newModel(1.f);
    // newModel.Rotate(verticalAngle, {0.f, 0.f, -1.f}).Rotate(-horizontalAngle, {0.f, 1.f, 0.f});
    model = newModel.Rotate(_horizontalAngle, {0.f, 1.f, 0.f}).Rotate(_verticalAngle, {0.f, 0.f, 1.f});
    UpdateViewMatrix(_isNormalHorizontalOrientation);

    RotateLines();
}

ZC_uptr<ZC_RS> ZCR_Orientation3D::MakeRSLine()
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

void ZCR_Orientation3D::WindowResizeCallback(float width, float height)
{
    perspective = ZC_Mat::Perspective(45.f, width / height, 0.1f, 5.f);
    perspView = perspective * view;
    RotateLines();
}

void ZCR_Orientation3D::UpdateViewMatrix(bool _isNormalHorizontalOrientation)
{
    if (isNormalHorizontalOrientation == _isNormalHorizontalOrientation) return;
    isNormalHorizontalOrientation = _isNormalHorizontalOrientation;
    view = isNormalHorizontalOrientation ? ZC_Mat::LookAt(ZC_Vec3<float>{5.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f})
        : ZC_Mat::LookAt(ZC_Vec3<float>{5.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, -1.f});
    perspView = perspective * view;
}

void ZCR_Orientation3D::RotateLines()
{
    ZC_Vec4<float> positivePointsOfLines[]
    {
        perspView * model * ZC_Vec4<float>(1.f, 0.f, 0.f, 1.f),   //  + x
        perspView * model * ZC_Vec4<float>(0.f, 1.f, 0.f, 1.f),   //  + y
        perspView * model * ZC_Vec4<float>(0.f, 0.f, 1.f, 1.f)    //  + z
    };

    const size_t threeVerticesByteSize = sizeof(ZC_Vec4<float>) * 3;
    upRSLine->buffers.begin()->BufferSubData(0, threeVerticesByteSize, positivePointsOfLines);
}