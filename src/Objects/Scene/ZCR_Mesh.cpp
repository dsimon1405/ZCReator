#include "ZCR_Mesh.h"

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
#include <ZC/Video/OpenGL/Renderer/ZC_Draw.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAOs.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RSNonTex.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZCR_Mesh::ZCR_Mesh(float totalLength)
    : upRendererSet(MakeRendererSet(totalLength)),
    upRSADSaxisXY(upRendererSet->Make_uptrRendererSetDrawingSet(nullptr, 0, 0)),
    upRSADSaxisXZ(upRendererSet->Make_uptrRendererSetDrawingSet(nullptr, 0, 0)),
    upRSADSaxisYZ(upRendererSet->Make_uptrRendererSetDrawingSet(nullptr, 0, 0))
{
    typedef typename ZC_Uniform::Name UName;
    upRSADSaxisXY->SetUniformsData(UName::unModel, ZC_Mat4<float>{1.f}.Begin());

    ZC_Mat4<float> modelXZ(1.f);
    modelXZ.Rotate(90.f, {1.f, 0.f, 0.f});
    upRSADSaxisXZ->SetUniformsData(UName::unModel, modelXZ.Begin());

    ZC_Mat4<float> modelYZ(1.f);
    modelYZ.Rotate(90.f, {0.f, 1.f, 0.f});
    upRSADSaxisYZ->SetUniformsData(UName::unModel, modelYZ.Begin());
    
    SwitchToCoordSystem(CoordSystem::xy);
}

void ZCR_Mesh::SwitchToCoordSystem(CoordSystem _coordSystem)
{
    if (coordSystem == _coordSystem) ZC_ErrorLogger::Err("Why called SwitchToCoordSystem() with same param?", __FILE__,__LINE__);

    switch (coordSystem)
    {
    case CoordSystem::none: break;
    case CoordSystem::xy: upRSADSaxisXY->SwitchToLvl(RSLvl::None); break;
    case CoordSystem::xz: upRSADSaxisXZ->SwitchToLvl(RSLvl::None); break;
    case CoordSystem::yz: upRSADSaxisYZ->SwitchToLvl(RSLvl::None); break;
    }
    coordSystem = _coordSystem;
    switch (coordSystem)
    {
    case CoordSystem::none: break;
    case CoordSystem::xy: upRSADSaxisXY->SwitchToLvl(RSLvl::Drawing); break;
    case CoordSystem::xz: upRSADSaxisXZ->SwitchToLvl(RSLvl::Drawing); break;
    case CoordSystem::yz: upRSADSaxisYZ->SwitchToLvl(RSLvl::Drawing); break;
    }
}

ZC_uptr<ZC_RendererSet> ZCR_Mesh::MakeRendererSet(float totalLength)
{
    //  (* 2 -> two vertices on one line) (+ 2 -> last line, if totalLength 10 than fifth line(center line) gonna be colored and 5 grey lines from sides
    ZC_DA<ZC_Vec3<float>> vertices((totalLength * 2 + 2) * 2);  //  last (* 2) -> pluss same count perpendicular lines 
    auto linePosition = -(totalLength / 2.f);
    auto axisLength = totalLength / 2.f;
    size_t i = 0;
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

    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawArrays>(GL_LINES, 0, vertices.size);

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::ZCR_LineMesh);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConData, vbo, nullptr, 0, 0);

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));
    return { new ZC_RSNonTex(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
}