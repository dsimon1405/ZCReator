#include "ZCR_Figures.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <Objects/Scene/ImGui/ZCR_IGWBM_Mode.h>
#include "ZCR_ActiveFigures.h"

ZCR_Figures::ZCR_Figures()
{
    pFigures = this;
}

ZCR_Figures::~ZCR_Figures()
{
    pFigures = nullptr;
}

void ZCR_Figures::CreateFigure(ZCR_FigureName figureName)
{
    if (ZCR_IGWBM_Mode::GetActiveSceneMode() != ZCR_SM_Model)
    {
        ZCR_IGWBM_Mode::SetActiveSceneMode(ZCR_SM_Model, false);
        ZCR_ActiveFigures::Clear(false);
        for (auto& figure : pFigures->figures) figure->ChangeSceneModeAndActivity(ZCR_SM_Model, false);
    }
    else ZCR_ActiveFigures::Clear(true);
    ZCR_ActiveFigures::Add((pFigures->figures.emplace_front(ZCR_FigureCreator::CreateFigure(figureName))).Get());
}

void ZCR_Figures::EraseFigure(ZCR_Figure* pFigure)
{

}

void ZCR_Figures::SwitchFiguresAndActiveFiguresToNewSceneMode(ZCR_SceneMode mode)
{
    for (auto& figure : pFigures->figures) figure->ChangeSceneMode(mode);
}