#include "ZCR_Mesh.h"

#include <ZC/Tools/Math/ZC_Mat4.h>
#include "ZCR_Scene.h"

void ZCR_Mesh::SetMeshSpecificCoordSystem(CoordSystem _coordSystem)
{
    specificCoordSystem = _coordSystem;
    _coordSystem == CS_Auto ? SetMeshAxis(ZCR_Scene::GetScene()->GetAxis()) : SwitchToCoordSystem(specificCoordSystem);
}

typename ZCR_Mesh::CoordSystem ZCR_Mesh::GetMeshSpecificCoordSystem()
{
    return specificCoordSystem;
}

ZCR_Mesh::ZCR_Mesh(float totalLength)
    : drawerSet(CreateDrawerSet(totalLength)),
    dsController_axisXY(drawerSet.MakeZC_DSController()),
    dsController_axisXZ(drawerSet.MakeZC_DSController()),
    dsController_axisYZ(drawerSet.MakeZC_DSController())
{
    dsController_axisXY.SetUniformsData(ZC_UN_unModel, ZC_Mat4<float>{1.f}.Begin());

    ZC_Mat4<float> modelXZ(1.f);
    modelXZ.Rotate(90.f, {1.f, 0.f, 0.f});
    dsController_axisXZ.SetUniformsData(ZC_UN_unModel, modelXZ.Begin());

    ZC_Mat4<float> modelYZ(1.f);
    modelYZ.Rotate(90.f, {0.f, 1.f, 0.f});
    dsController_axisYZ.SetUniformsData(ZC_UN_unModel, modelYZ.Begin());
    
    SwitchToCoordSystem(CoordSystem::CS_XY);
}

void ZCR_Mesh::SetMeshAxis(ZCR_Axis axis)
{
    if (specificCoordSystem != CS_Auto) return;  //  the user specified a specific coordinate system
    switch (axis)
    {
    case ZCR_A_PositiveX: SwitchToCoordSystem(CS_YZ); break;
    case ZCR_A_NegativeX: SwitchToCoordSystem(CS_YZ); break;
    case ZCR_A_PositiveY: SwitchToCoordSystem(CS_XZ); break;
    case ZCR_A_NegativeY: SwitchToCoordSystem(CS_XZ); break;
    default: SwitchToCoordSystem(CS_XY); break;
    }
}

ZC_DrawerSet ZCR_Mesh::CreateDrawerSet(float totalLength)
{
    //  (* 2 -> two vertices on one line) (+ 2 -> last line, if totalLength 10 than fifth line(center line) gonna be colored and 5 grey lines from sides
    ZC_DA<ZC_Vec3<float>> vertices((static_cast<ulong>(totalLength) * 2 + 2) * 2);  //  last (* 2) -> pluss same count perpendicular lines 
    ZC_DA<ZC_Vec3<uchar>> colors(vertices.size);
    auto linePosition = -(totalLength / 2.f);
    auto axisLength = totalLength / 2.f;
    ulong i = 0;
    for ( ; i < vertices.size / 2; )
    {
        vertices[i] = ZC_Vec3<float>(linePosition, axisLength, 0);
        vertices[i + 1] = ZC_Vec3<float>(linePosition, -axisLength, 0);
        colors[i] = static_cast<int>(linePosition) % 10 == 0 ? ZC_Vec3<uchar>(115, 115, 115) : ZC_Vec3<uchar>(94, 94, 94);
        colors[i + 1] = colors[i];
        i += 2;
        ++linePosition;
    }
    linePosition = -(totalLength / 2.f);
    for ( ; i < vertices.size; )
    {
        vertices[i] = ZC_Vec3<float>(axisLength, linePosition, 0);
        vertices[i + 1] = ZC_Vec3<float>(-axisLength, linePosition, 0);
        colors[i] = static_cast<int>(linePosition) % 10 == 0 ? ZC_Vec3<uchar>(115, 115, 115) : ZC_Vec3<uchar>(94, 94, 94);
        colors[i + 1] = colors[i];
        i += 2;
        ++linePosition;
    }    

    ZC_Buffer vbo(GL_ARRAY_BUFFER);
    vbo.GLNamedBufferStorage(vertices.BytesSize() + colors.BytesSize(), vertices.pHead, GL_DYNAMIC_STORAGE_BIT);
    vbo.GLNamedBufferSubData(vertices.BytesSize(), colors.BytesSize(), colors.pHead);

    auto upGLDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawArrays>(GL_LINES, 0, vertices.size);

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ShPN_ZCR_LineMesh);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, nullptr, 0, vertices.size);

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));
    return ZC_DrawerSet(pShPIS, std::move(vao), std::move(upGLDraw), std::move(buffers));
}

void ZCR_Mesh::SwitchToCoordSystem(CoordSystem _coordSystem)
{
    if (coordSystem == _coordSystem) return;

    switch (coordSystem)
    {
    case CS_XY: dsController_axisXY.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None); break;
    case CS_XZ: dsController_axisXZ.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None); break;
    case CS_YZ: dsController_axisYZ.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_None); break;
    default: break;
    }

    coordSystem = _coordSystem;
    
    switch (coordSystem)
    {
    case CS_XY: dsController_axisXY.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing); break;
    case CS_XZ: dsController_axisXZ.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing); break;
    case CS_YZ: dsController_axisYZ.SwitchToDrawLvl(ZC_RL_Default, ZC_DL_Drawing); break;
    default: break;
    }
}