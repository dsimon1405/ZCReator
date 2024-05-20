#include "ZCR_Figure.h"

#include <Objects/Scene/ImGui/ZCR_IGWBM_Mode.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>

ZCR_Figure::ZCR_Figure(std::string&& _name, ZC_DA<ZC_Quad>&& _quads, ZC_DA<ZC_Triangle>&& _triangles, ZC_DA<int>&& _normals)
    : ZCR_VBO(std::move(_quads), std::move(_triangles), std::move(_normals)),
    matModel(GetPointerOnStencilBorderData()),
    id(idCounter++),
    name(_name + "##" + std::to_string(id))
{
    this->MakeAllPointsActive();

    dscPoint.SetUniformsData(ZC_UN_unModel, matModel.model.Begin());
    dscLine.SetUniformsData(ZC_UN_unModel, matModel.model.Begin());
    dscSurface.SetUniformsData(ZC_UN_unModel, matModel.model.Begin());


    static bool first = true;
    if (first)
    {
        matModel.SetTranslate(ZCR_MMA_X, 3.f, false);
        matModel.SetTranslate(ZCR_MMA_Y, 1.f, false);
        matModel.SetTranslate(ZCR_MMA_Z, 2.f, true);
        // alpha = 0.8f
        // useLight = false

        first = false;
    }
}

// ZCR_Figure::ZCR_Figure(ZCR_Figure&& f)
//     : ZCR_VBO(dynamic_cast<ZCR_VBO&&>(f)),
//     ZCR_Point(dynamic_cast<ZCR_Point&&>(f)),
//     ZCR_Line(dynamic_cast<ZCR_Line&&>(f)),
//     ZCR_Surface(dynamic_cast<ZCR_Surface&&>(f)),
//     matModel(f.matModel),
//     name(std::move(f.name)),
//     alpha(f.alpha),
//     useLight(f.useLight),
//     isActiveOnScene(f.isActiveOnScene)
// {
//     dsConPoint.SetUniformsData(ZC_UN_unModel, matModel.model.Begin());
//     dsConLine.SetUniformsData(ZC_UN_unModel, matModel.model.Begin());
//     dsConSurface.SetUniformsData(ZC_UN_unModel, matModel.model.Begin());
//     dsConSurface.SetUniformsData(ZC_UN_unAlpha, &alpha);
//     dsConSurface.SetUniformsData(ZCR_UN_unUseLight, &useLight);
// }

bool ZCR_Figure::operator == (ZCR_Figure* fig) const noexcept
{
    return this == fig;
}

void ZCR_Figure::SetSceneActivity(bool _isActiveOnScene)
{
    if (isActiveOnScene == _isActiveOnScene) return;
    this->isActiveOnScene = _isActiveOnScene;
    ChangeSceneMode(ZCR_IGWBM_Mode::GetActiveSceneMode());
}

void ZCR_Figure::ChangeSceneModeAndActivity(ZCR_SceneMode sceneMode, bool _isActiveOnScene)
{
    this->isActiveOnScene = _isActiveOnScene;
    ChangeSceneMode(sceneMode);
}

const std::string& ZCR_Figure::GetName() const noexcept
{
    return name;
}

void ZCR_Figure::SetName(std::string&& _name)
{
    name = _name + "##" + std::to_string(id);
}

bool ZCR_Figure::IsActiveOnScene() const noexcept
{
    return isActiveOnScene;
}

void ZCR_Figure::ChangeSceneMode(ZCR_SceneMode sceneMode)
{
    this->ChangeSceneModeTriangle(sceneMode);
    this->ChangeSceneModePoint(sceneMode);
    this->ChangeSceneModeLine(sceneMode);
}

ZC_RSPDStencilBorderData* ZCR_Figure::GetPointerOnStencilBorderData()
{
    for (auto& personalData : this->dscSurface.personalData)
        if (*personalData == ZC_RSPDC_stencilBorder) return &(dynamic_cast<ZC_RSPDStencilBorder*>(personalData.Get())->stencilBorderData);
    
    return nullptr;
}