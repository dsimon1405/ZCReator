#include "ZCR_IGWBM_Mode.h"

#include <imgui.h>
#include <Objects/Figure/ZCR_Figures.h>

ZCR_IGWBM_Mode::ZCR_IGWBM_Mode(float startX, float startY)
    : ZC_IGWBM(K_M, true, false, false, "Mode", false, 470.f, 30.f, startX, startY, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Top_Pixel, true,
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse)
{
    pIGWBM_Mode = this;
}

ZCR_IGWBM_Mode::~ZCR_IGWBM_Mode()
{
    pIGWBM_Mode = nullptr;
}

ZCR_SceneMode ZCR_IGWBM_Mode::GetActiveSceneMode() noexcept
{
    return pIGWBM_Mode->activeSceneMode;
}

void ZCR_IGWBM_Mode::SetActiveSceneMode(ZCR_SceneMode _sceneMode, bool changeFiguresState) noexcept
{
    if (pIGWBM_Mode->activeSceneMode != _sceneMode)
    {
        ZCR_SceneMode previousMode = pIGWBM_Mode->activeSceneMode;
        pIGWBM_Mode->activeSceneMode = _sceneMode;  //  make it to actualize state before call from ZCR_IGWBM_M_Edit or ZCR_IGWBM_M_Model or ZCR_IGWBM_M_Sculpting::VDeactivateIGWBM()
        //  if new mode ZCR_SM_None, just stops draw current state, if new mode legal make it draw
        _sceneMode == ZCR_SM_None ? pIGWBM_Mode->ChangeDrawState(previousMode, false) : pIGWBM_Mode->ChangeDrawState(_sceneMode, true);
        if (changeFiguresState) ZCR_Figures::SwitchFiguresAndActiveFiguresToNewSceneMode(pIGWBM_Mode->activeSceneMode);
    }
}

bool ZCR_IGWBM_Mode::IsOneOfModes_ActiveBM() noexcept
{
    return pIGWBM_Mode->igwbmModel.IsActiveIGWBM() || pIGWBM_Mode->igwbmEdit.IsActiveIGWBM() || pIGWBM_Mode->igwbmSculpting.IsActiveIGWBM();
}

void ZCR_IGWBM_Mode::ActivateIGWBM()
{
    pIGWBM_Mode->ActivateIGW();
}

void ZCR_IGWBM_Mode::VDrawWindowIGW()
{
    if (ImGui::BeginTable("Modes", 4, 0))
    {
        auto drawMode = [this](ZCR_SceneMode drawingSceneMode, const char* name)
        {
            ImGui::TableNextColumn();
            if (ImGui::RadioButton(name, activeSceneMode == drawingSceneMode)) SetActiveSceneMode(drawingSceneMode, true);
        };

        bool isActiveBM = this->IsActiveBM();
        drawMode(ZCR_SM_None, isActiveBM ? activeSceneMode == ZCR_SM_None ? "None(Esc)" : "None(N)" : "None");
        drawMode(ZCR_SM_Model, isActiveBM ? activeSceneMode == ZCR_SM_Model ? "Model(Esc)" : "Model(M)" : "Model");
        drawMode(ZCR_SM_Edit, isActiveBM ? activeSceneMode == ZCR_SM_Edit ? "Edit(Esc)" : "Edit(E)" : "Edit");
        drawMode(ZCR_SM_Sculpting, isActiveBM ? activeSceneMode == ZCR_SM_Sculpting ? "Sculpting(Esc)" : "Sculpting(S)" : "Sculpting");
        ImGui::EndTable();
    }
}

void ZCR_IGWBM_Mode::VCallButtonDownBM(ZC_ButtonID buttonId, float time)
{
    switch (buttonId)
    {
    case K_N: SetActiveSceneMode(ZCR_SM_None, true); break;
    case K_M: SetActiveSceneMode(ZCR_SM_Model, true); break;
    case K_E: SetActiveSceneMode(ZCR_SM_Edit, true); break;
    case K_S: SetActiveSceneMode(ZCR_SM_Sculpting, true); break;
    default: break;
    }
}

void ZCR_IGWBM_Mode::ChangeDrawState(ZCR_SceneMode sceneMode, bool needDraw)
{
    switch (sceneMode)
    {
    case ZCR_SM_Model: needDraw ? igwbmModel.ActivateIGW() : igwbmModel.NeedDrawIGW(false); break;
    case ZCR_SM_Edit: needDraw ? igwbmEdit.ActivateIGW() : igwbmEdit.NeedDrawIGW(false); break;
    case ZCR_SM_Sculpting: needDraw ? igwbmSculpting.ActivateIGW() : igwbmSculpting.NeedDrawIGW(false); break;
    default: break;
    }

    this->NeedDrawIGW(false); //  stop drawing Mode's window
}