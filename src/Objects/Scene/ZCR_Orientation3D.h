#pragma once

#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>
#include <ZC/Tools/Container/ZC_DA.h>
#include <ZC/Tools/Math/ZC_Mat4.h>

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
    ZC_uptr<ZC_RendererSet> upRendSetLine;
    ZC_RSController rsControllerLine;

    ZC_uptr<ZC_RendererSet> rendSetAxise;
    ZC_RSController rsControllerAxises[6];
    
    bool isNormalHorizontalOrientation = true;
    ZC_Mat4<float> perspective,
        view,
        perspView,
        modelLine,
        modelRotateQuads;

    ZC_uptr<ZC_RendererSet> MakeRSLine();
    ZC_uptr<ZC_RendererSet> MakeRendererSetAxice();
    void WindowResizeCallback(float width, float height);
    void UpdateViewMatrix(bool _isNormalHorizontalOrientation);
    void RotateLinesAndTranslateQuads();
};