#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/GUI/ZC_GUI__Window.h>
#include <ZC/GUI/ZC_GUI__CheckBox.h>
#include <ZC/GUI/ZC_GUI__Switch.h>
#include <ZC/GUI/ZC_GUI__Text.h>
#include <ZC/GUI/Button/ZC_GUI__ButtonNumberText.h>

struct ZCR_GUI_W_SceneOptions
{
    ZC_GUI__WinImmutable win;

    ZCR_GUI_W_SceneOptions();

        //  callback ZCR_GUI_W_Scene::cmt_scene
    void OpenWindow(float);

private:
        //  gui section
    ZC_GUI__CheckBox cb_gui;   //  on/off guie

        //  axis section
    ZC_GUI__Text t_axis;
    ZC_GUI__SwitchText st_axis;     //  choose drawing axis

        //  camera section
    ZC_GUI__ButtonNumberText<float> bnt_nearPlane;
    // ZC_GUI__ClickMouse cm_default_nearPlane;
    ZC_GUI__ButtonNumberText<float> bnt_farPlane;
    // ZC_GUI__ClickMouse cm_default_farPlane;
    ZC_GUI__ButtonNumberText<float> bnt_fovy;
    // ZC_GUI__ClickMouse cm_default_fovy;

    float nearPlane_default = 0.01f;
    float farPlane_default = 100.f;
    float fovy_default = 45.f;

        //  gui section
    void Callback__chB_gui(bool isOn);

        //  axis section
        //  change gui show state on (Ctrl + G)
    void Ctrl_G(ZC_ButtonID butotnId, float);
    void Callback__st_axis(uint index);

        //  camera section
    void Callback__bnt_nearPlane(float near_plane){}
    void Callback__bnt_farPlane(float near_plane){}
    void Callback__bnt_fovy(float near_plane){}
};