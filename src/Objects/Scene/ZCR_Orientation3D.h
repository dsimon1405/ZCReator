#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RS.h>
#include <ZC/Tools/Container/ZC_DA.h>

struct ZCR_Orientation3D
{
    enum Axice
    {
        Axice_posX = 0,
        Axice_negX,
        Axice_posY,
        Axice_negY,
        Axice_posZ,
        Axice_negZ,
    };

    ZCR_Orientation3D(float windowAspect);
    void MoveAroundObject(float _horizontalAngle, float _verticalAngle, bool _isNormalHorizontalOrientation);

private:
    ZC_uptr<ZC_RS> upRSLine;
    ZC_uptr<ZC_RendererSetAndDrawingSet> upRSASDLine;

    ZC_uptr<ZC_RendererSet> rendSetAxices;
    ZC_uptr<ZC_RendererSetAndDrawingSet> RSASDAxices[6];
    
    bool isNormalHorizontalOrientation = true;
    ZC_Mat4<float> perspective,
        view,
        perspView,
        modelLine,
        modelRotateQuads;

    ZC_uptr<ZC_RS> MakeRSLine();
    ZC_uptr<ZC_RendererSet> MakeRendererSetAxice();
    void WindowResizeCallback(float width, float height);
    void UpdateViewMatrix(bool _isNormalHorizontalOrientation);
    void RotateLinesAndTranslateQuads();
};