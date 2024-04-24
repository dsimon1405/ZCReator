#include "ZCR_Mesh.h"

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
#include <ZC/Video/OpenGL/Renderer/ZC_Draw.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAOs.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Math/ZC_Mat4.h>

ZCR_Mesh::ZCR_Mesh(float totalLength)
    : upRendererSet(MakeRendererSet(totalLength)),
    rsController_axisXY(upRendererSet->MakeZC_RSController()),
    rsController_axisXZ(upRendererSet->MakeZC_RSController()),
    rsController_axisYZ(upRendererSet->MakeZC_RSController())
{
    ZC_RSPDUniformData unModelXY(ZC_UN_unModel, ZC_Mat4<float>{1.f}.Begin());
    rsController_axisXY.SetData(ZC_RSPDC_uniforms, &unModelXY);

    ZC_Mat4<float> modelXZ(1.f);
    modelXZ.Rotate(90.f, {1.f, 0.f, 0.f});
    ZC_RSPDUniformData unModelXZ(ZC_UN_unModel, modelXZ.Begin());
    rsController_axisXZ.SetData(ZC_RSPDC_uniforms, &unModelXZ);

    ZC_Mat4<float> modelYZ(1.f);
    modelYZ.Rotate(90.f, {0.f, 1.f, 0.f});
    ZC_RSPDUniformData unModelYZ(ZC_UN_unModel, modelYZ.Begin());
    rsController_axisYZ.SetData(ZC_RSPDC_uniforms, &unModelYZ);
    
    SwitchToCoordSystem(CoordSystem::xy);
}

void ZCR_Mesh::SetAxis(ZCR_Axis axis)
{
    switch (axis)
    {
    case ZCR_A_PositiveX: SwitchToCoordSystem(); break;
    case ZCR_A_NegativeX: SwitchToCoordSystem(); break;
    case ZCR_A_PositiveY: SwitchToCoordSystem(); break;
    case ZCR_A_NegativeY: SwitchToCoordSystem(); break;
    case ZCR_A_PositiveZ: SwitchToCoordSystem(); break;
    case ZCR_A_NegativeZ: SwitchToCoordSystem(); break;
    case ZCR_A_Free: SwitchToCoordSystem(); break;
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
    if (coordSystem == _coordSystem) ZC_ErrorLogger::Err("Why called SwitchToCoordSystem() with same param?", __FILE__,__LINE__);

    switch (coordSystem)
    {
    case CoordSystem::none: break;
    case CoordSystem::xy: rsController_axisXY.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_None); break;
    case CoordSystem::xz: rsController_axisXZ.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_None); break;
    case CoordSystem::yz: rsController_axisYZ.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_None); break;
    }
    coordSystem = _coordSystem;
    switch (coordSystem)
    {
    case CoordSystem::none: break;
    case CoordSystem::xy: rsController_axisXY.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_Drawing); break;
    case CoordSystem::xz: rsController_axisXZ.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_Drawing); break;
    case CoordSystem::yz: rsController_axisYZ.SwitchToDrawLvl(ZC_FB_Default, ZC_DL_Drawing); break;
    }
}