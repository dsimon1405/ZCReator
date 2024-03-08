#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RendererSetAndDrawingSet.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RS.h>

struct ZCR_Orientation3D
{
    ZCR_Orientation3D(float windowAspect);
    void MoveAroundObject(float _horizontalAngle, float _verticalAngle, bool _isNormalHorizontalOrientation);

private:
    ZC_uptr<ZC_RS> upRSLine;
    ZC_uptr<ZC_RendererSetAndDrawingSet> upRASDLine;
    
    bool isNormalHorizontalOrientation = true;
    ZC_Mat4<float> perspective,
        view,
        perspView,
        model;

    ZC_uptr<ZC_RS> MakeRSLine();
    void WindowResizeCallback(float width, float height);
    void UpdateViewMatrix(bool _isNormalHorizontalOrientation);
    void RotateLines();
};