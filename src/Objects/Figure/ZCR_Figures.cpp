#include "ZCR_Figures.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>
// #include <Objects/Scene/ImGui/ZCR_IGWBM_Mode.h>
#include <Objects/Scene/GUI/Mode/ZCR_GUI_SceneMode.h>
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
    if (ZCR_GUI_SceneMode::GetActiveSceneMode() != ZCR_SM_Model)
    {
        ZCR_GUI_SceneMode::SetActiveSceneMode(ZCR_SM_Model, false);
        for (auto& figure : pFigures->figures) figure->ChangeSceneModeAndActivity(ZCR_SM_Model, false);
    }
    // if (ZCR_IGWBM_Mode::GetActiveSceneMode() != ZCR_SM_Model)
    // {
    //     ZCR_IGWBM_Mode::SetActiveSceneMode(ZCR_SM_Model, false);
    //     for (auto& figure : pFigures->figures) figure->ChangeSceneModeAndActivity(ZCR_SM_Model, false);
    // }
    
    MakeFigureActive((pFigures->figures.emplace_back(ZCR_FigureCreator::CreateFigure(figureName))).Get());
}

void ZCR_Figures::EraseFigure(ZCR_Figure* pFigure)
{

}

void ZCR_Figures::SwitchSceneMode(ZCR_SceneMode mode)
{
    for (auto& figure : pFigures->figures) figure->ChangeSceneMode(mode);
}

ZCR_Figure* ZCR_Figures::GetActiveFigure()
{
    return pFigures->pActiveFigure;
}

void ZCR_Figures::MakeFigureActive(ZCR_Figure* pFigure)
{
    if (pFigures->pActiveFigure == pFigure) return;
    if (pFigures->pActiveFigure) pFigures->pActiveFigure->SetSceneActivity(false);
    pFigure->SetSceneActivity(true);;
    pFigures->pActiveFigure = pFigure;
    pFigures->igwFigures.SetNameBuffer(pFigure);
}