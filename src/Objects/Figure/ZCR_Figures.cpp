#include "ZCR_Figures.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <Objects/Scene/ZCR_Scene.h>
#include "ZCR_ActiveFigures.h"

void ZCR_Figures::CreateFigure(typename ZCR_Figure::Name name)
{
    if (ZCR_Scene::GetActiveSceneMode() != SceneMode::Model)
    {
        ZCR_Scene::SetActiveSceneMode(SceneMode::Model, false);
        ZCR_ActiveFigures::Clear(false);
        for (auto& upFigure : figures) upFigure->ChangeSceneModeAndActivity(SceneMode::Model, false);
    }
    else ZCR_ActiveFigures::Clear(true);
    ZCR_ActiveFigures::Add(figures.emplace_front(ZC_uptrMake<ZCR_Figure>(name)).Get());
}

void ZCR_Figures::EraseFigure(ZCR_Figure* pFigure)
{

}

void ZCR_Figures::SwitchFiguresAndActiveFiguresToNewSceneMode(SceneMode mode)
{
    for (auto& upFigure : figures) upFigure->ChangeSceneMode(mode);
}