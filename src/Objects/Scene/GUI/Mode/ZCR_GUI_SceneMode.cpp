#include "ZCR_GUI_SceneMode.h"

#include <Objects/Figure/ZCR_Figures.h>

ZCR_GUI_SceneMode::ZCR_GUI_SceneMode()
    : sdd_mode({ L"Model", L"Edit", L"Sculpting" }, 0, 0, 0, { &ZCR_GUI_SceneMode::SwitchActiveModeWindowDrawState, this })
{
    pSceneMode = this;
}

ZCR_SceneMode ZCR_GUI_SceneMode::GetActiveSceneMode()
{
    return pSceneMode->sceneMode_active;
}

void ZCR_GUI_SceneMode::SetActiveSceneMode(ZCR_SceneMode sceneMode, bool changeFiguresState)
{
    if (sceneMode == pSceneMode->sceneMode_active) return;

    auto lambChangeActiveWindowDrawState = [](ZCR_GUI_SceneMode* pSM)
    {
        switch (int(pSM->sceneMode_active))
        {
        case ZCR_SM_Model: pSM->w_modeModel.win.SetDrawState(!(pSM->w_modeModel.win.IsDrawing())); break;
        case ZCR_SM_Edit: pSM->w_modeEdit.win.SetDrawState(!(pSM->w_modeEdit.win.IsDrawing())); break;
        case ZCR_SM_Sculpting: pSM->w_modeSculpting.win.SetDrawState(!(pSM->w_modeSculpting.win.IsDrawing())); break;
        }
    };
    lambChangeActiveWindowDrawState(pSceneMode);
    pSceneMode->sceneMode_active = sceneMode;
    lambChangeActiveWindowDrawState(pSceneMode);

    if (changeFiguresState) ZCR_Figures::SwitchSceneMode(pSceneMode->sceneMode_active);
}

void ZCR_GUI_SceneMode::SwitchActiveModeWindowDrawState(uint index)
{
    SetActiveSceneMode(static_cast<ZCR_SceneMode>(index), true);
}