#include "ZCR_IGWBM_MainMenu.h"

#include <imgui.h>

ZCR_IGWBM_MainMenu::ZCR_IGWBM_MainMenu()
    : ZC_IGWBM(K_UNKNOWN, false, true, false, "MainMenu", true, 135.f, 80.f, orthoIndentX, orthoIndentY, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Top_Pixel, false,
        ImGuiWindowFlags_NoMove
         | ImGuiWindowFlags_NoBackground
          | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse),
    igwbmMode(orthoIndentX, orthoIndentY + 46.f)
{}

void ZCR_IGWBM_MainMenu::VDrawWindowIGW()
{
    // ImGui::ShowDemoWindow();
    
    bool isActiveBM = this->IsActiveBM();   //  active for button calls

    if (ImGui::Button(isActiveBM ? "Interface(I)    " : igwbmInterface.IsActiveIGWBM() ? "Interface(I/Esc)" : "Interface       ", { 120.f, 20.f })) igwbmInterface.ActivateIGW();
    if (ImGui::Button(isActiveBM ? "Create(C)    " : igwbmCreate.IsActiveIGWBM() ? "Create(C/Esc)" : "Create       ", { 100.f, 20.f })) igwbmCreate.ActivateIGW();
    
    if (!igwbmMode.IsActiveIGWBM())
    {
        bool isOneOfModesActiveBM = igwbmMode.IsOneOfModes_ActiveBM();
        std::string mode;
        switch (igwbmMode.GetActiveSceneMode())
        {
        case ZCR_SM_None: mode = isActiveBM || isOneOfModesActiveBM ? "None(M)" : "None"; break;
        case ZCR_SM_Model: mode = isActiveBM || isOneOfModesActiveBM ? "Model(M)" : "Model"; break;
        case ZCR_SM_Edit: mode = isActiveBM || isOneOfModesActiveBM ? "Edit(M)" : "Edit"; break;
        case ZCR_SM_Sculpting: mode = isActiveBM || isOneOfModesActiveBM ? "Sculpting(M)" : "Sculpting"; break;
        }
        if (ImGui::RadioButton(mode.c_str(), true)) igwbmMode.ActivateIGW();
    }
}

void ZCR_IGWBM_MainMenu::VCallButtonDownBM(ZC_ButtonID buttonId, float time)
{
    switch (buttonId)
    {
    case K_I: igwbmInterface.ActivateIGW(); break;
    case K_C: igwbmCreate.ActivateIGW(); break;
    case K_M: igwbmMode.ActivateIGW(); break;
    default: break;
    }
}