#pragma once

#include "ZCR_Point.h"
#include "ZCR_Line.h"
#include "ZCR_Surface.h"
#include <Objects/Scene/ZCR_Scene.h>

class ZCR_FigureSet : public ZCR_Point, public ZCR_Line, public ZCR_Surface
{
public:
    ZCR_FigureSet(ZC_sptr<ZC_DA<ZC_Quad>>&& _quads, ZC_sptr<ZC_DA<ZC_Triangle>>&& _triangles, ZC_sptr<ZC_DA<int>>&& _normals);

    void SwitchToSceneMode(ZCR_SceneModes sceneMode, bool isActiveOnScene);
    void TranslateModelMatrix(const ZC_Vec3<float>& trans);
    void SetAlpha(float alpha);
    void SetUseLight(bool _useLight);
    bool IsActiveOnScene() const noexcept;

private:
    ZC_sptr<ZC_Mat4<float>> spMatModel = ZC_sptrMake<ZC_Mat4<float>>(1.f);
    float alpha = 1.f;
    int useLight = true;    //  uses as int in uniform
    bool isActiveOnScene = false;

    void SwitchGLElementOnRendererLevel(ZC_DrawLevel triangle, ZC_DrawLevel line, ZC_DrawLevel point);
};