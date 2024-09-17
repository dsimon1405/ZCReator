#include "ZCR_GUI_W_SceneOptions.h"

#include <ZC/GUI/ZC__GUI.h>
#include <ZC/Events/ZC_Events.h>
#include <Objects/Scene/ZCR_Scene.h>

ZCR_GUI_W_SceneOptions::ZCR_GUI_W_SceneOptions()
    : win(ZC_WOIData(400.f, 400.f, 0.f, 0.f, ZC_WOIF__X_Center | ZC_WOIF__Y_Center), ZC_GUI_WF__EscapeClose | ZC_GUI_WF__Frame),
    cb_gui(L"Show GUI (Ctr + G)", true, { &ZCR_GUI_W_SceneOptions::Callback__chB_gui, this }),
    t_axis(L"Mesh:", true, 0, ZC_GUI_TextAlignment::Left),
    st_axis({ L"None", L"X_Y", L"X_Z", L"Y_Z", L"Auto" }, 0.f, 0.f, true, 5.f, { &ZCR_GUI_W_SceneOptions::Callback__st_axis, this }, 4),
    bnt_nearPlane(ZC_GUI_ButtonNumber<float>(0.f, 0.f, nearPlane_default, nearPlane_default, 100000.f, 0.1f, 2.f, 2, ZC_GUI_TextAlignment::Center, { &ZCR_GUI_W_SceneOptions::Callback__bnt_nearPlane, this }), L"Near plane", 5.f),
    // cm_default_nearPlane(),     //  STOPED HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bnt_farPlane(ZC_GUI_ButtonNumber<float>(0.f, 0.f, farPlane_default, nearPlane_default, 100000, 0.1f, 2.f, 2, ZC_GUI_TextAlignment::Center, { &ZCR_GUI_W_SceneOptions::Callback__bnt_farPlane, this }), L"Far plane", 5.f),
    // cm_default_farPlane(),
    bnt_fovy(ZC_GUI_ButtonNumber<float>(0.f, 0.f, fovy_default, 0.1f, 180.f, 1.f, 3.f, 2, ZC_GUI_TextAlignment::Center, { &ZCR_GUI_W_SceneOptions::Callback__bnt_fovy, this }), L"Fovy", 5.f)
    // cm_default_fovy()
{
    float left_offset = 20.f;
    float top_offset = 10.f;
    float y_section_distance = 15.f;
    win.AddRow(ZC_GUI_Row(ZC_GUI_RowParams(left_offset, ZC_GUI_RowParams::Left, top_offset, 0.f), { cb_gui.GetObj() }));
    win.AddRow(ZC_GUI_Row(ZC_GUI_RowParams(0.f, ZC_GUI_RowParams::Center, y_section_distance, 0.f), { t_axis.GetObj() }));
    win.AddRow(ZC_GUI_Row(ZC_GUI_RowParams(0.f, ZC_GUI_RowParams::Center, 5.f, 0.f), { st_axis.GetObj() }));

    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_G, { &ZCR_GUI_W_SceneOptions::Ctrl_G, this }, nullptr);
}

void ZCR_GUI_W_SceneOptions::OpenWindow(float)
{
    win.SetDrawState(true);
}

void ZCR_GUI_W_SceneOptions::Callback__chB_gui(bool isOn)
{
    ZC__GUI::SetState(isOn);
}

void ZCR_GUI_W_SceneOptions::Ctrl_G(ZC_ButtonID butotnId, float)
{
    if (ZC_Events::IsButtonPressed(ZC_ButtonID::K_LCTRL) || ZC_Events::IsButtonPressed(ZC_ButtonID::K_RCTRL))
        cb_gui.ChangeState(true);
}

void ZCR_GUI_W_SceneOptions::Callback__st_axis(uint index)
{
    ZCR_Scene::pScene->SetMeshSpecificCoordSystem(ZCR_Mesh::CoordSystem(index));
}

