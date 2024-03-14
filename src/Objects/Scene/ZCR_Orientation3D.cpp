#include "ZCR_Orientation3D.h"

#include <ZC/Video/OpenGL/Renderer/ZC_RSNotTextured.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RSTextured.h>
#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC/Tools/Math/ZC_Mat.h>
#include "ZCR_Camera.h"

ZCR_Orientation3D::ZCR_Orientation3D(float windowAspect)
    : upRSLine(MakeRSLine()),
    upRSASDLine(upRSLine->Make_uptrRendererSetDrawingSet(nullptr, 0, 0)),
    rendSetAxices(MakeRendererSetAxice()),
    RSASDAxices
        {
            rendSetAxices->Make_uptrRendererSetDrawingSet("", 0, 0),
            rendSetAxices->Make_uptrRendererSetDrawingSet("", 0, 0),
            rendSetAxices->Make_uptrRendererSetDrawingSet("", 0, 0),
            rendSetAxices->Make_uptrRendererSetDrawingSet("", 0, 0),
            rendSetAxices->Make_uptrRendererSetDrawingSet("", 0, 0),
            rendSetAxices->Make_uptrRendererSetDrawingSet("", 0, 0)
        },
    perspective(ZC_Mat::Perspective(45.f, windowAspect, 0.1f, 10.f)),
    view(ZC_Mat::LookAt(ZC_Vec3<float>{5.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f})),
    perspView(perspective * view)
{
    ZC_Events::ConnectWindowResize({ &ZCR_Orientation3D::WindowResizeCallback, this });
    
    upRSASDLine->SwitchToLvl(ZC_RendererSet::Level::Drawing);

    for (size_t i = 0; i < 6; i++) RSASDAxices[i]->SwitchToLvl(ZC_RendererSet::Level::Drawing);
}
float horizontalAngle = 0, verticalAngle = 0;
void ZCR_Orientation3D::MoveAroundObject(float _horizontalAngle, float _verticalAngle, bool _isNormalHorizontalOrientation)
{
    horizontalAngle = _horizontalAngle;
    verticalAngle = _verticalAngle;
    UpdateViewMatrix(_isNormalHorizontalOrientation);


    RotateLinesAndTranslateQuads();
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

    return { new ZC_RSNotTextured(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers)) };
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

    typename ZC_ShProgs::ShPInitSet* pShPIS = ZC_ShProgs::Get(ZC_ShProgs::Name::ZCR_QuadOrientation3D);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConData, vbo, nullptr, 0, 24);   //  24 vertices amount

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));

    auto votc = pShPIS->texSets.GetCreator();
    votc.Add(ZC_Textures::LoadTexture2D(ZC_FSPath(ZCR_ZCRTexturePath).append("sceneIcons.png").c_str()));
    std::vector<ZC_RSTextured::TexSet> texSets;
    texSets.emplace_back("", votc.GetVector());
    
    return { new ZC_RSTextured(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers), std::move(texSets)) };
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
    newModel.Rotate(verticalAngle, {0.f, 0.f, -1.f}).Rotate(-horizontalAngle, {0.f, 1.f, 0.f});
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

    const size_t threeVerticesByteSize = sizeof(ZC_Vec4<float>) * 3;
    upRSLine->buffers.begin()->BufferSubData(0, threeVerticesByteSize, positivePointsOfLines);

    ZC_Mat4<float> mod(1.f);
    mod.Rotate(verticalAngle, {0.f, 0.f, -1.f}).Rotate(horizontalAngle, {0.f, -1.f, 0.f});
    if (!isNormalHorizontalOrientation) mod.Rotate(180.f, {1.f,0.f,0.f});   //  turn around texture
    
    ZC_Mat4<float> posX(1.f);
    posX = perspView * posX.Translate({ positiveX[0], positiveX[1], positiveX[2]}) * mod;
    RSASDAxices[Axice_posX]->SetUniformsData(ZC_Uniform::Name::unModel, &posX);
    ZC_Mat4<float> negX(1.f);
    negX = perspView * negX.Translate({ -positiveX[0], -positiveX[1], -positiveX[2]}) * mod;
    RSASDAxices[Axice_negX]->SetUniformsData(ZC_Uniform::Name::unModel, &negX);
    ZC_Mat4<float> posY(1.f);
    posY = perspView * posY.Translate({ positiveY[0], positiveY[1], positiveY[2]}) * mod;
    RSASDAxices[Axice_posY]->SetUniformsData(ZC_Uniform::Name::unModel, &posY);
    ZC_Mat4<float> negY(1.f);
    negY = perspView * negY.Translate({ -positiveY[0], -positiveY[1], -positiveY[2]}) * mod;
    RSASDAxices[Axice_negY]->SetUniformsData(ZC_Uniform::Name::unModel, &negY);
    ZC_Mat4<float> posZ(1.f);
    posZ = perspView * posZ.Translate({ positiveZ[0], positiveZ[1], positiveZ[2]}) * mod;
    RSASDAxices[Axice_posZ]->SetUniformsData(ZC_Uniform::Name::unModel, &posZ);
    ZC_Mat4<float> negZ(1.f);
    negZ = perspView * negZ.Translate({ -positiveZ[0], -positiveZ[1], -positiveZ[2]}) * mod;
    RSASDAxices[Axice_negZ]->SetUniformsData(ZC_Uniform::Name::unModel, &negZ);
}