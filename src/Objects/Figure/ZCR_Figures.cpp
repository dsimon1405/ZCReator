#include "ZCR_Figures.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <Objects/Scene/ImGui/ZCR_IGWBM_Mode.h>
#include "ZCR_ActiveFigures.h"

void ZCR_Figures::CreateFigure(FigureName figureName)
{
    if (ZCR_IGWBM_Mode::GetActiveSceneMode() != ZCR_SM_Model)
    {
        ZCR_IGWBM_Mode::SetActiveSceneMode(ZCR_SM_Model, false);
        ZCR_ActiveFigures::Clear(false);
        for (auto& upFigure : figures) upFigure->ChangeSceneModeAndActivity(ZCR_SM_Model, false);
    }
    else ZCR_ActiveFigures::Clear(true);
    ZCR_ActiveFigures::Add(figures.emplace_front(ZC_uptrMake<ZCR_Figure>(figureName)).Get());
}

void ZCR_Figures::EraseFigure(ZCR_Figure* pFigure)
{

}

void ZCR_Figures::SwitchFiguresAndActiveFiguresToNewSceneMode(ZCR_SceneMode mode)
{
    for (auto& upFigure : figures) upFigure->ChangeSceneMode(mode);
}