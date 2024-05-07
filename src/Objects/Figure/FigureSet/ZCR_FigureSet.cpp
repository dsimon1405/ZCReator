#include "ZCR_FigureSet.h"

#include <Renderer/ZCR_RenderLevel.h>
#include <Objects/Scene/ImGui/ZCR_IGWBM_Mode.h>

ZCR_FigureSet::ZCR_FigureSet(ZC_sptr<ZC_DA<ZC_Quad>>&& _quads, ZC_sptr<ZC_DA<ZC_Triangle>>&& _triangles, ZC_sptr<ZC_DA<int>>&& _normals)
    : ZCR_VBO(std::move(_quads), std::move(_triangles), std::move(_normals))
{
    FillVBO();

    //  active points
    std::list<Points*> allActivePoints;
    for (auto& point : *spPoints) allActivePoints.emplace_back(&point);
    MakePointsActive(std::move(allActivePoints));

    dsControllerPoint.SetUniformsData(ZC_UN_unModel, spMatModel->Begin());
    dsControllerSurface.SetUniformsData(ZC_UN_unAlpha, &alpha);
    dsControllerSurface.SetUniformsData(ZCR_UN_unUseLight, &useLight);

    dsControllerLine.SetUniformsData(ZC_UN_unModel, spMatModel->Begin());
    dsControllerSurface.SetUniformsData(ZC_UN_unModel, spMatModel->Begin());

    static bool first = true;
    if (first)
    {
        spMatModel->Translate(3.f, 1.f, 2.f);
        // alpha = 0.8f;
        // useLight = false;

        first = false;
    }
}

void ZCR_FigureSet::SwitchToSceneMode(ZCR_SceneMode sceneMode, bool _isActiveOnScene)
{
    isActiveOnScene = _isActiveOnScene;
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

void ZCR_FigureSet::TranslateModelMatrix(const ZC_Vec3<float>& trans)
{
    spMatModel->Translate(trans);
}

void ZCR_FigureSet::SetAlpha(float _alpha)
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

void ZCR_FigureSet::SetUseLight(bool _useLight)
{
    useLight = _useLight;
}

bool ZCR_FigureSet::IsActiveOnScene() const noexcept
{
    return isActiveOnScene;
}

void ZCR_FigureSet::SwitchGLElementOnRendererLevel(ZC_DrawerLevel triangle, ZC_DrawerLevel point, ZC_DrawerLevel line)
{
    SwitchRSControllerTriangle(triangle);
    SwitchRSControllerPoint(point);
    SwitchRSControllerLine(line);
}