#pragma once

#include "FigureData/ZCR_Point.h"
#include "FigureData/ZCR_Line.h"
#include "FigureData/ZCR_Surface.h"
#include <Objects/Scene/ZCR_SceneMode.h>
#include <ZC/Tools/Math/ZC_Mat4.h>
#include "FigureData/ZCR_MatModel.h"    //  add

class ZCR_Figure : public ZCR_Point, public ZCR_Line, public ZCR_Surface
{
public:
    ZCR_Figure(std::string&& _name, ZC_DA<ZC_Quad>&& _quads, ZC_DA<ZC_Triangle>&& _triangles, ZC_DA<int>&& _normals);
    ZCR_Figure(ZCR_Figure&& f);

    bool operator == (ZCR_Figure* fig) const noexcept;

    void Translate(const ZC_Vec3<float>& trans);

    void ChangeSceneMode(ZCR_SceneMode sceneMode);
    void ChangeSceneActivity(bool _isActiveOnScene);
    void ChangeSceneModeAndActivity(ZCR_SceneMode sceneMode, bool _isActiveOnScene);

    void SetAlpha(float alpha);
    void SetUseLight(bool _useLight);
    bool IsActiveOnScene() const noexcept;

private:
    std::string name;
    ZC_Mat4<float> matModel { 1.f };
    float alpha = 1.f;
    int useLight = true;    //  uses as int in uniform
    bool isActiveOnScene = false;

    void SwitchGLElementOnRendererLevel(ZC_DrawerLevel triangle, ZC_DrawerLevel line, ZC_DrawerLevel point);
};