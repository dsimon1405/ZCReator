#pragma once

#include <ZC/GUI/ZC_GUI_WinImmutable.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/GUI/ZC_GUI_ButtonMouseText.h>
#include <Objects/Scene/ZCR_Axis.h>

struct ZCR_GUI_Orientation3D : public ZC_GUI_WinImmutable
{
    struct AxisButton
    {
        ZC_GUI_ButtonMouseText bmt;
        ZCR_Axis axis;
        ZC_Vec4<float> start_pos;

        AxisButton(const std::wstring& wstr_axis, ZCR_Axis _axis, const ZC_Vec4<float>& _start_pos)
            : bmt(0.f, 0.f, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(0.f, ZC_GUI_TextForButton::Indent::Center), wstr_axis, true, 0, ZC_GUI_TextAlignment::Left)),
            axis(_axis),
            start_pos(_start_pos)
        {} 
    };

    static inline const float win_size = 100.f;
    ZC_EC ecCamChangePos;
    AxisButton axisButtons[6];

    ZCR_GUI_Orientation3D()
        : ZC_GUI_WinImmutable(ZC_WOIData(win_size, win_size, 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel), ZC_GUI_WF__NeedDraw | ZC_GUI_WF__NoBackground | ZC_GUI_WF__Stacionar),
        ecCamChangePos(ZC_Events::ConnectActiveCameraChangePosition({ &ZCR_GUI_Orientation3D::CameraChangePosition, this })),
        axisButtons
            {
                AxisButton(L"+X", ZCR_Axis::ZCR_A_PositiveX, { 1.f, 0.f, 0.f, 1.f }),
                AxisButton(L"-X", ZCR_Axis::ZCR_A_NegativeX, { -1.f, 0.f, 0.f, 1.f }),
                AxisButton(L"+Y", ZCR_Axis::ZCR_A_PositiveY, { 0.f, 1.f, 0.f, 1.f }),
                AxisButton(L"-Y", ZCR_Axis::ZCR_A_NegativeY, { 0.f, -1.f, 0.f, 1.f }),
                AxisButton(L"+Z", ZCR_Axis::ZCR_A_PositiveZ, { 0.f, 0.f, 1.f, 1.f }),
                AxisButton(L"-Z", ZCR_Axis::ZCR_A_NegativeZ, { 0.f, 0.f, -1.f, 1.f })
            }
    {}

    void CameraChangePosition(const ZC_Vec3<float>& camPos)
    {

    }
};



    // struct AxisButton : public ZC_GUI_ButtonMouseText
    // {
    //     ZCR_Axis axis;
    //     ZC_Vec4<float> start_pos;

    //     AxisButton(const std::wstring& wstr_axis, ZCR_Axis _axis, const ZC_Vec4<float>& _start_pos)
    //         : ZC_GUI_ButtonBase(ZC_GUI_ObjData(0.f, 0.f, 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None),
    //         ZC_GUI_ButtonMouseText(0.f, 0.f, ZC_GUI_BF__None, ZC_GUI_TextForButton(ZC_GUI_TextForButton::Indent(0.f, ZC_GUI_TextForButton::Indent::Center), wstr_axis, true, 0, ZC_GUI_TextAlignment::Left)),
    //         axis(_axis),
    //         start_pos(_start_pos)
    //     {}
    // };