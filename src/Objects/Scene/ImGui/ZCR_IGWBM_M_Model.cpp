#include "ZCR_IGWBM_M_Model.h"

#include "ZCR_IGWBM_Mode.h"
#include <imgui.h>

ZCR_IGWBM_M_Model::ZCR_IGWBM_M_Model()
    : ZC_IGWBM(K_M, false, false, false, "Model", false, 50.f, 100.f, 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Center, true,
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)
{}

void ZCR_IGWBM_M_Model::VDrawWindowIGW()
{

}

void ZCR_IGWBM_M_Model::VCallButtonDownBM(ZC_ButtonID buttonId, float time)
{
    switch (buttonId)
    {
    case K_M: ZCR_IGWBM_Mode::ActivateIGWBM(); break;
    default: break;
    }
}

void ZCR_IGWBM_M_Model::VDeactivateIGWBM()
{
    //  if the active scene mode is the same as ours, then VDeactivateIGWBM() is called to exit all scene modes
    if (ZCR_IGWBM_Mode::GetActiveSceneMode() == ZCR_SM_Model) ZCR_IGWBM_Mode::SetActiveSceneMode(ZCR_SM_None, true);
}