#pragma once

#include <ZC/GUI/ZC_GUI__Window.h>

#define ZCR_mode_icon_size 40
struct ZC_GUI_W_ModeModel
{
    ZC_GUI__WinImmutable win;
        //  must be ZC_GUI__SwitchKeyboardUV
    
    // ZC_GUI_W_ModeModel();

    // ZC_GUI_W_ModeModel::
    ZC_GUI_W_ModeModel()
        : win(ZC_WOIData(ZCR_mode_icon_size, ZCR_mode_icon_size, 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Center), ZC_GUI_WF__Stacionar | ZC_GUI_WF__NeedDraw)
    {}
};

struct ZC_GUI_W_ModeEdit
{
    ZC_GUI__WinImmutable win;
        //  must be ZC_GUI__SwitchKeyboardUV

    // ZC_GUI_W_ModeEdit();

    // ZC_GUI_W_ModeEdit::
    ZC_GUI_W_ModeEdit()
        : win(ZC_WOIData(ZCR_mode_icon_size, ZCR_mode_icon_size, 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Center), ZC_GUI_WF__Stacionar)
    {}
};

struct ZC_GUI_W_ModeSculpting
{
    ZC_GUI__WinImmutable win;
        //  must be ZC_GUI__SwitchKeyboardUV

    // ZC_GUI_W_ModeSculpting();

    // ZC_GUI_W_ModeSculpting::
    ZC_GUI_W_ModeSculpting()
        : win(ZC_WOIData(ZCR_mode_icon_size, ZCR_mode_icon_size, 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Center), ZC_GUI_WF__Stacionar)
    {}
};

#include <Objects/Scene/ZCR_SceneMode.h>
#include <ZC/GUI/ZC_GUI__Switch.h>

struct ZCR_GUI_SceneMode
{
    ZC_GUI__SwitchDropDown sdd_mode;

    ZCR_GUI_SceneMode();

    static ZCR_SceneMode GetActiveSceneMode();
    static void SetActiveSceneMode(ZCR_SceneMode sceneMode, bool changeFiguresState);

private:
    static inline ZCR_GUI_SceneMode* pSceneMode;

    ZCR_SceneMode sceneMode_active = ZCR_SM_Model;

    ZC_GUI_W_ModeModel w_modeModel;
    ZC_GUI_W_ModeEdit w_modeEdit;
    ZC_GUI_W_ModeSculpting w_modeSculpting;

        //  ssd_mode callback
    void SwitchActiveModeWindowDrawState(uint index);
};