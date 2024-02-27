#pragma once

#include "ZCR_Point.h"
#include "ZCR_Line.h"
#include "ZCR_Surface.h"
#include <Objects/Scene/ZCR_Scene.h>

class ZCR_FigureSet : public ZCR_Point, public ZCR_Line, public ZCR_Surface
{
public:
    ZCR_FigureSet(ZC_sptr<ZC_DA<ZC_Quad>>&& _quads, ZC_sptr<ZC_DA<ZC_Triangle>>&& _triangles, ZC_sptr<ZC_DA<int>>&& _normals);
    // ZCR_FigureSet(ZCR_FigureSet&& fs);

    typedef typename ZCR_Scene::SceneMode SceneMode;
    void SwitchToSceneMode(SceneMode sceneMode, bool isActiveOnScene);
    void TranslateModelMatrix(const ZC_Vec3<float>& trans);

private:
    ZC_sptr<ZC_Mat4<float>> spMatModel = ZC_sptrMake<ZC_Mat4<float>>(1.f);

    typedef typename ZC_RendererSet::Level RSLvl;
    void SwitchGLElementOnRSLevel(RSLvl triangle, RSLvl line, RSLvl point);
};