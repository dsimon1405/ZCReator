#include "ZCR_Mesh.h"

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
#include <ZC/Video/OpenGL/Renderer/ZC_Draw.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAOs.h>
#include <ZC/Tools/Math/ZC_Mat4.h>

void ZCR_Mesh::SetMeshSpecificCoordSystem(CoordSystem _coordSystem)
{
    specificCoordSystem = _coordSystem;
    SwitchToCoordSystem(specificCoordSystem);
}

ZCR_Mesh::ZCR_Mesh(float totalLength)
    : upRendererSet(MakeRendererSet(totalLength)),
    rsController_axisXY(upRendererSet->MakeZC_RSController()),
    rsController_axisXZ(upRendererSet->MakeZC_RSController()),
    rsController_axisYZ(upRendererSet->MakeZC_RSController())
{
    rsController_axisXY.SetUniformsData(ZC_UN_unModel, ZC_Mat4<float>{1.f}.Begin());

    ZC_Mat4<float> modelXZ(1.f);
    modelXZ.Rotate(90.f, {1.f, 0.f, 0.f});
    rsController_axisXZ.SetUniformsData(ZC_UN_unModel, modelXZ.Begin());

    ZC_Mat4<float> modelYZ(1.f);
    modelYZ.Rotate(90.f, {0.f, 1.f, 0.f});
    rsController_axisYZ.SetUniformsData(ZC_UN_unModel, modelYZ.Begin());
    
    SwitchToCoordSystem(CoordSystem::CS_XY);
}

void ZCR_Mesh::SetMeshAxis(ZCR_Axis axis)
{
    if (specificCoordSystem != CS_Nothing) return;  //  the user specified a specific coordinate system
    switch (axis)
    {
    case ZCR_A_PositiveX: SwitchToCoordSystem(CS_YZ); break;
    case ZCR_A_NegativeX: SwitchToCoordSystem(CS_YZ); break;
    case ZCR_A_PositiveY: SwitchToCoordSystem(CS_XZ); break;
    case ZCR_A_NegativeY: SwitchToCoordSystem(CS_XZ); break;
    case ZCR_A_PositiveZ: SwitchToCoordSystem(CS_XY); break;
    case ZCR_A_NegativeZ: SwitchToCoordSystem(CS_XY); break;
    case ZCR_A_Free: SwitchToCoordSystem(CS_XY); break;
    }
}

ZC_uptr<ZC_RenderSet> ZCR_Mesh::MakeRendererSet(float totalLength)
{
    //  (* 2 -> two vertices on one line) (+ 2 -> last line, if totalLength 10 than fifth line(center line) gonna be colored and 5 grey lines from sides
    ZC_DA<ZC_Vec3<float>> vertices((static_cast<ulong>(totalLength) * 2 + 2) * 2);  //  last (* 2) -> pluss same count perpendicular lines 
    auto linePosition = -(totalLength / 2.f);
    auto axisLength = totalLength / 2.f;
    ulong i = 0;
    for ( ; i < vertices.size / 2; )
    {
        vertices[i++] = ZC_Vec3<float>(linePosition, axisLength, 0);
        vertices[i++] = ZC_Vec3<float>(linePosition, -axisLength, 0);
        ++linePosition;
    }
    linePosition = -(totalLength / 2.f);
    for ( ; i < vertices.size; )
    {
        vertices[i++] = ZC_Vec3<float>(axisLength, linePosition, 0);
        vertices[i++] = ZC_Vec3<float>(-axisLength, linePosition, 0);
        ++linePosition;
    }

    ZC_Buffer vbo(GL_ARRAY_BUFFER);
    vbo.BufferData(vertices.BytesSize(), vertices.pHead, GL_STATIC_DRAW);

    auto upGLDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawArrays>(GL_LINES, 0, vertices.size);

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ShPN_ZCR_LineMesh);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, nullptr, 0, 0);

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));
    return ZC_RenderSet::CreateUptr(pShPIS, std::move(vao), std::move(upGLDraw), std::move(buffers));
}

void ZCR_Mesh::SwitchToCoordSystem(CoordSystem _coordSystem)
{
    if (coordSystem == _coordSystem) return;

    switch (coordSystem)
    {
    case CoordSystem::CS_None: break;
    case CoordSystem::CS_XY: rsController_axisXY.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_None); break;
    case CoordSystem::CS_XZ: rsController_axisXZ.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_None); break;
    case CoordSystem::CS_YZ: rsController_axisYZ.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_None); break;
    default: break;
    }
    coordSystem = _coordSystem;
    switch (coordSystem)
    {
    case CoordSystem::CS_None: break;
    case CoordSystem::CS_XY: rsController_axisXY.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_Drawing); break;
    case CoordSystem::CS_XZ: rsController_axisXZ.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_Drawing); break;
    case CoordSystem::CS_YZ: rsController_axisYZ.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_Drawing); break;
    default: break;
    }
}