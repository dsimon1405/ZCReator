#include "ZCR_Orientation3D.h"

#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC/Tools/Math/ZC_Mat.h>
#include "ZCR_Camera.h"
#include <ZC/Tools/Math/ZC_Mat.h>

ZCR_Orientation3D::ZCR_Orientation3D(float windowAspect)
    : upRendSetLine(MakeRSLine()),
    rsControllerLine(upRendSetLine->MakeZC_RSController()),
    rendSetAxise(MakeRendererSetAxice()),
    rsControllerAxises
        {
            rendSetAxise->MakeZC_RSController(0, {}),   //  0 is texture id set in MakeRendererSetAxice()
            rendSetAxise->MakeZC_RSController(0, {}),
            rendSetAxise->MakeZC_RSController(0, {}),
            rendSetAxise->MakeZC_RSController(0, {}),
            rendSetAxise->MakeZC_RSController(0, {}),
            rendSetAxise->MakeZC_RSController(0, {})
        },
    perspective(ZC_Mat::Perspective(45.f, windowAspect, 0.1f, 10.f)),
    view(ZC_Mat::LookAt(ZC_Vec3<float>{5.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f})),
    perspView(perspective * view)
{
    ZC_Events::ConnectWindowResize({ &ZCR_Orientation3D::WindowResizeCallback, this });
    
    rsControllerLine.SwitchToLvl(ZC_RL_Drawing);

    for (ulong i = 0; i < 6; i++) rsControllerAxises[i].SwitchToLvl(ZC_RL_Drawing);
}
float verticalAngle = 0, horizontalAngle = 0;
void ZCR_Orientation3D::MoveAroundObject(float _verticalAngle, float _horizontalAngle, bool _isNormalHorizontalOrientation)
{
    verticalAngle = _verticalAngle;
    horizontalAngle = _horizontalAngle;
    UpdateViewMatrix(_isNormalHorizontalOrientation);


    RotateLinesAndTranslateQuads();
}

ZC_uptr<ZC_RendererSet> ZCR_Orientation3D::MakeRSLine()
{
    uchar colors[]
    {
        255, 0, 0,
        0, 255, 0,
        0, 0, 255,
    };

    ZC_Buffer vbo(GL_ARRAY_BUFFER);
    const ulong threeVerticesByteSize = sizeof(ZC_Vec4<float>) * 3;
    vbo.BufferData(threeVerticesByteSize + sizeof(colors), nullptr, GL_DYNAMIC_DRAW);
    vbo.BufferSubData(threeVerticesByteSize, sizeof(colors), colors);
    
    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawArrays>(GL_POINTS, 0, 3);   //  draw as GL_POINTS but drawing 3 lines in geometry shader

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ShPN_ZCR_LineOrientation3D);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, nullptr, 0, 3);   //  last param = 3, vertices amount

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));

    return ZC_RendererSet::CreateUptr(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers));
}
#include <Objects/Scene/ImGui/ZCR_IconTexture.h>
#include "ZC/Video/OpenGL/Texture/ZC_Textures.h"
#include <File/ZCR_File.h>
ZC_uptr<ZC_RendererSet> ZCR_Orientation3D::MakeRendererSetAxice()
{
    float positionAndTexCoords[]
    {
        0.f, -0.2f, -0.2f,      //  bl  +x
        0.f,  0.2f, -0.2f,      //  br
        0.f,  0.2f,  0.2f,      //  tr
        0.f, -0.2f,  0.2f,      //  tl
        
        0.f, -0.2f, -0.2f,      //  bl  -x
        0.f,  0.2f, -0.2f,      //  br
        0.f,  0.2f,  0.2f,      //  tr
        0.f, -0.2f,  0.2f,      //  tl
        
        0.f, -0.2f, -0.2f,      //  bl
        0.f,  0.2f, -0.2f,      //  br
        0.f,  0.2f,  0.2f,      //  tr
        0.f, -0.2f,  0.2f,      //  tl
        
        0.f, -0.2f, -0.2f,      //  bl
        0.f,  0.2f, -0.2f,      //  br
        0.f,  0.2f,  0.2f,      //  tr
        0.f, -0.2f,  0.2f,      //  tl
        
        0.f, -0.2f, -0.2f,      //  bl
        0.f,  0.2f, -0.2f,      //  br
        0.f,  0.2f,  0.2f,      //  tr
        0.f, -0.2f,  0.2f,      //  tl
        
        0.f, -0.2f, -0.2f,      //  bl
        0.f,  0.2f, -0.2f,      //  br
        0.f,  0.2f,  0.2f,      //  tr
        0.f, -0.2f,  0.2f,      //  tl


        ZCR_IconTexture::tcCreateCube.tl.x, ZCR_IconTexture::tcCreateCube.br.y,     //  bl  +x
        ZCR_IconTexture::tcCreateCube.br.x, ZCR_IconTexture::tcCreateCube.br.y,     //  br
        ZCR_IconTexture::tcCreateCube.br.x, ZCR_IconTexture::tcCreateCube.tl.y,     //  tr
        ZCR_IconTexture::tcCreateCube.tl.x, ZCR_IconTexture::tcCreateCube.tl.y,     //  tl

        ZCR_IconTexture::tcCreateCube.tl.x, ZCR_IconTexture::tcCreateCube.tl.y,     //  bl  -x
        ZCR_IconTexture::tcCreateCube.br.x, ZCR_IconTexture::tcCreateCube.tl.y,     //  br
        ZCR_IconTexture::tcCreateCube.br.x, ZCR_IconTexture::tcCreateCube.br.y,     //  tr
        ZCR_IconTexture::tcCreateCube.tl.x, ZCR_IconTexture::tcCreateCube.br.y,     //  tl

        ZCR_IconTexture::tcCreateCone.tl.x, ZCR_IconTexture::tcCreateCone.br.y,     //  bl  +y
        ZCR_IconTexture::tcCreateCone.br.x, ZCR_IconTexture::tcCreateCone.br.y,     //  br
        ZCR_IconTexture::tcCreateCone.br.x, ZCR_IconTexture::tcCreateCone.tl.y,     //  tr
        ZCR_IconTexture::tcCreateCone.tl.x, ZCR_IconTexture::tcCreateCone.tl.y,     //  tl

        ZCR_IconTexture::tcCreateCone.tl.x, ZCR_IconTexture::tcCreateCone.tl.y,     //  bl  -y
        ZCR_IconTexture::tcCreateCone.br.x, ZCR_IconTexture::tcCreateCone.tl.y,     //  br
        ZCR_IconTexture::tcCreateCone.br.x, ZCR_IconTexture::tcCreateCone.br.y,     //  tr
        ZCR_IconTexture::tcCreateCone.tl.x, ZCR_IconTexture::tcCreateCone.br.y,     //  tl

        ZCR_IconTexture::tcCreateCylinder.tl.x, ZCR_IconTexture::tcCreateCylinder.br.y,     //  bl  +z
        ZCR_IconTexture::tcCreateCylinder.br.x, ZCR_IconTexture::tcCreateCylinder.br.y,     //  br
        ZCR_IconTexture::tcCreateCylinder.br.x, ZCR_IconTexture::tcCreateCylinder.tl.y,     //  tr
        ZCR_IconTexture::tcCreateCylinder.tl.x, ZCR_IconTexture::tcCreateCylinder.tl.y,     //  tl

        ZCR_IconTexture::tcCreateCylinder.tl.x, ZCR_IconTexture::tcCreateCylinder.tl.y,     //  bl  -z
        ZCR_IconTexture::tcCreateCylinder.br.x, ZCR_IconTexture::tcCreateCylinder.tl.y,     //  br
        ZCR_IconTexture::tcCreateCylinder.br.x, ZCR_IconTexture::tcCreateCylinder.br.y,     //  tr
        ZCR_IconTexture::tcCreateCylinder.tl.x, ZCR_IconTexture::tcCreateCylinder.br.y,     //  tl
    };

    ZC_Buffer vbo(GL_ARRAY_BUFFER);
    vbo.BufferData(sizeof(positionAndTexCoords), positionAndTexCoords, GL_STATIC_DRAW);
    
    auto upDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawArrays>(GL_TRIANGLE_FAN, 0, 24);   //  24 vertices amount

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ShPN_ZCR_QuadOrientation3D);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, nullptr, 0, 24);   //  24 vertices amount

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));

    auto votc = pShPIS->texSets.GetCreator();
    votc.Add(ZC_Textures::LoadTexture2D(ZC_FSPath(ZCR_ZCRTexturePath).append("sceneIcons.png").string().c_str()));
    std::forward_list<ZC_TexturesSet> texSets;
    texSets.emplace_front(0, votc.GetVector());
    
    return ZC_RendererSet::CreateUptr(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers), std::move(texSets));
}

void ZCR_Orientation3D::WindowResizeCallback(float width, float height)
{
    perspective = ZC_Mat::Perspective(45.f, width / height, 0.1f, 10.f);
    perspView = perspective * view;
    RotateLinesAndTranslateQuads();
}

void ZCR_Orientation3D::UpdateViewMatrix(bool _isNormalHorizontalOrientation)
{
    ZC_Mat4<float> newModel(1.f);
    newModel.Rotate(horizontalAngle, {0.f, 0.f, -1.f}).Rotate(verticalAngle, {0.f, -1.f, 0.f});
    ZC_Vec4<float> camPos = newModel * ZC_Vec4<float>(5.f, 0.f, 0.f, 1.f);  //  5 distance to center
    isNormalHorizontalOrientation = _isNormalHorizontalOrientation;
    view = isNormalHorizontalOrientation ? ZC_Mat::LookAt(ZC_Vec3<float>{camPos[0], camPos[1], camPos[2]}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f})
        : ZC_Mat::LookAt(ZC_Vec3<float>{camPos[0], camPos[1], camPos[2]}, {0.f, 0.f, 0.f}, {0.f, 0.f, -1.f});
    perspView = perspective * view;
}

void ZCR_Orientation3D::RotateLinesAndTranslateQuads()
{
    static ZC_Vec4<float> positiveX(1.f, 0.f, 0.f, 1.f);
    static ZC_Vec4<float> positiveY(0.f, 1.f, 0.f, 1.f);
    static ZC_Vec4<float> positiveZ(0.f, 0.f, 1.f, 1.f);
    ZC_Vec4<float> positivePointsOfLines[]
    {
        perspView * positiveX,   //  + x
        perspView * positiveY,   //  + y
        perspView * positiveZ,   //  + z
    };

    const ulong threeVerticesByteSize = sizeof(ZC_Vec4<float>) * 3;
    upRendSetLine->buffers.begin()->BufferSubData(0, threeVerticesByteSize, positivePointsOfLines);

    ZC_Mat4<float> mod(1.f);
    mod.Rotate(horizontalAngle, {0.f, 0.f, -1.f}).Rotate(verticalAngle, {0.f, -1.f, 0.f});
    if (!isNormalHorizontalOrientation) mod.Rotate(180.f, {1.f,0.f,0.f});   //  turn around texture
    
    ZC_Mat4<float> posX(1.f);
    posX = perspView * posX.Translate(positiveX[0], positiveX[1], positiveX[2]) * mod;
    ZC_RSPDUniformData unModelPosX(ZC_UN_unModel, posX.Begin());
    rsControllerAxises[Axice_posX].SetData(ZC_RSPDC_uniforms, &unModelPosX);
    ZC_Mat4<float> negX(1.f);
    negX = perspView * negX.Translate(-positiveX[0], -positiveX[1], -positiveX[2]) * mod;
    ZC_RSPDUniformData unModelNegX(ZC_UN_unModel, negX.Begin());
    rsControllerAxises[Axice_negX].SetData(ZC_RSPDC_uniforms, &unModelNegX);
    ZC_Mat4<float> posY(1.f);
    posY = perspView * posY.Translate(positiveY[0], positiveY[1], positiveY[2]) * mod;
    ZC_RSPDUniformData unModelPosY(ZC_UN_unModel, posY.Begin());
    rsControllerAxises[Axice_posY].SetData(ZC_RSPDC_uniforms, &unModelPosY);
    ZC_Mat4<float> negY(1.f);
    negY = perspView * negY.Translate(-positiveY[0], -positiveY[1], -positiveY[2]) * mod;
    ZC_RSPDUniformData unModelNegY(ZC_UN_unModel, negY.Begin());
    rsControllerAxises[Axice_negY].SetData(ZC_RSPDC_uniforms, &unModelNegY);
    ZC_Mat4<float> posZ(1.f);
    posZ = perspView * posZ.Translate(positiveZ[0], positiveZ[1], positiveZ[2]) * mod;
    ZC_RSPDUniformData unModelPosZ(ZC_UN_unModel, posZ.Begin());
    rsControllerAxises[Axice_posZ].SetData(ZC_RSPDC_uniforms, &unModelPosZ);
    ZC_Mat4<float> negZ(1.f);
    negZ = perspView * negZ.Translate(-positiveZ[0], -positiveZ[1], -positiveZ[2]) * mod;
    ZC_RSPDUniformData unModelNegZ(ZC_UN_unModel, negZ.Begin());
    rsControllerAxises[Axice_negZ].SetData(ZC_RSPDC_uniforms, &unModelNegZ);
}