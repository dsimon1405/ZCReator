#include "ZCR_Figure.h"

#include <Renderer/ZCR_RenderLevel.h>
#include <Objects/Scene/ImGui/ZCR_IGWBM_Mode.h>

ZCR_Figure::ZCR_Figure(std::string&& _name, ZC_DA<ZC_Quad>&& _quads, ZC_DA<ZC_Triangle>&& _triangles, ZC_DA<int>&& _normals)
    : ZCR_VBO(std::move(_quads), std::move(_triangles), std::move(_normals)),
    name(std::move(_name))
{
    FillVBO();

    dsConPoint.SetUniformsData(ZC_UN_unModel, matModel.Begin());
    dsConLine.SetUniformsData(ZC_UN_unModel, matModel.Begin());
    dsConSurface.SetUniformsData(ZC_UN_unModel, matModel.Begin());
    dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alpha);
    dsConSurface.SetUniformsData(ZCR_UN_unUseLight, &useLight);


    static bool first = true;
    if (first)
    {
        matModel.Translate(3.f, 1.f, 2.f);
        // alpha = 0.8f;
        // useLight = false;

        first = false;
    }
}

ZCR_Figure::ZCR_Figure(ZCR_Figure&& f)
    : ZCR_VBO(dynamic_cast<ZCR_VBO&&>(f)),
    ZCR_Point(dynamic_cast<ZCR_Point&&>(f)),
    ZCR_Line(dynamic_cast<ZCR_Line&&>(f)),
    ZCR_Surface(dynamic_cast<ZCR_Surface&&>(f)),
    name(std::move(f.name)),
    matModel(f.matModel),
    alpha(f.alpha),
    useLight(f.useLight),
    isActiveOnScene(f.isActiveOnScene)
{
    dsConPoint.SetUniformsData(ZC_UN_unModel, matModel.Begin());
    dsConLine.SetUniformsData(ZC_UN_unModel, matModel.Begin());
    dsConSurface.SetUniformsData(ZC_UN_unModel, matModel.Begin());
    dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alpha);
    dsConSurface.SetUniformsData(ZCR_UN_unUseLight, &useLight);
}

bool ZCR_Figure::operator == (ZCR_Figure* fig) const noexcept
{
    return this == fig;
}

void ZCR_Figure::ChangeSceneMode(ZCR_SceneMode sceneMode)
{
    switch (sceneMode)
    {
    case ZCR_SM_None: SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_None, ZC_DL_None); break;
    case ZCR_SM_Model:
    {
        ZC_DrawerLevel surfaceLevel =
            isActiveOnScene ?
                alpha == 1.f ? ZC_DL_StencilBorder
                    : alpha == 0.f ? (ZC_DrawerLevel)ZC_DL_None : ZCR_DL_AlphaBlending
                : ZC_DL_Drawing;
        SwitchGLElementOnRendererLevel(surfaceLevel, ZC_DL_None, ZC_DL_None);
        break;
    }
    case ZCR_SM_Edit:
    {
        if (isActiveOnScene) SwitchGLElementOnRendererLevel(alpha == 0.f ? (ZC_DrawerLevel)ZC_DL_None : ZCR_DL_AlphaBlending,
            ZC_DL_Drawing, ZC_DL_Drawing);
        else SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_None, ZC_DL_None);
        break;
    }
    case ZCR_SM_Sculpting:
    {
        if (isActiveOnScene) SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_None, ZC_DL_None);
        else SwitchGLElementOnRendererLevel(ZC_DL_None, ZC_DL_None, ZC_DL_None);
        break;
    }
    }
}

void ZCR_Figure::ChangeSceneActivity(bool _isActiveOnScene)
{
    if (isActiveOnScene == _isActiveOnScene) return;
    isActiveOnScene = _isActiveOnScene;
    isActiveOnScene ? this->MakeAllPointsActive() : this->MakeAllPointsPassive();
    ChangeSceneMode(ZCR_IGWBM_Mode::GetActiveSceneMode());
}

void ZCR_Figure::ChangeSceneModeAndActivity(ZCR_SceneMode sceneMode, bool _isActiveOnScene)
{
    if (isActiveOnScene != _isActiveOnScene)
    {
        isActiveOnScene = _isActiveOnScene;
        isActiveOnScene ? this->MakeAllPointsActive() : this->MakeAllPointsPassive();
    }
    ChangeSceneMode(sceneMode);
}

void ZCR_Figure::Translate(const ZC_Vec3<float>& trans)
{
    matModel.Translate(trans);
}

void ZCR_Figure::SetAlpha(float _alpha)
{
    if (alpha == _alpha) return;
    alpha = _alpha < 0.f ? 0.f
        :_alpha > 1.f ? 1.f
        : _alpha;

    switch (ZCR_IGWBM_Mode::GetActiveSceneMode())
    {
    case ZCR_SM_None: SwitchGLElementOnRendererLevel(ZC_DL_Drawing, ZC_DL_None, ZC_DL_None);
    case ZCR_SM_Model: SwitchRSControllerTriangle(isActiveOnScene ? alpha == 1.f ? ZC_DL_StencilBorder
        : alpha == 0.f ? (ZC_DrawerLevel)ZC_DL_None : ZCR_DL_AlphaBlending : ZC_DL_Drawing); break;
    case ZCR_SM_Edit: SwitchRSControllerTriangle(isActiveOnScene ? alpha == 0.f ? (ZC_DrawerLevel)ZC_DL_None : ZCR_DL_AlphaBlending : ZC_DL_Drawing); break;
    case ZCR_SM_Sculpting: SwitchRSControllerTriangle(isActiveOnScene ? ZC_DL_Drawing : ZC_DL_None); break;
    }
}

void ZCR_Figure::SetUseLight(bool _useLight)
{
    useLight = _useLight;
}

bool ZCR_Figure::IsActiveOnScene() const noexcept
{
    return isActiveOnScene;
}

void ZCR_Figure::SwitchGLElementOnRendererLevel(ZC_DrawerLevel triangle, ZC_DrawerLevel point, ZC_DrawerLevel line)
{
    SwitchRSControllerTriangle(triangle);
    SwitchRSControllerPoint(point);
    SwitchRSControllerLine(line);
}