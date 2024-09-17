#pragma once

#include <ZC/GUI/Backend/Window/ZC_GUI_WinImmutable.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouseText.h>
#include <Objects/Scene/ZCR_Axis.h>

struct ZCR_GUI_Orientation3D : public ZC_GUI_WinImmutable
{
    struct AxisButton : public ZC_GUI_ButtonMouseText
    {
        static inline AxisButton* pActiveAxis = nullptr;
        static inline float win_max_depth = 0.f;

        ZCR_Axis axis;
        ZC_Vec3<float> start_pos;
        float temp_depth = 0.f;

        AxisButton(const std::wstring& wstr_axis, ZCR_Axis _axis, const ZC_Vec3<float>& _start_pos, uint _color_default, uint color_under_cursor)
            : ZC_GUI_ButtonBase(ZC_GUI_ObjData(0.f, 0.f, 0, ZC_GUI_IconUV::arrow_right, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, {_color_default, color_under_cursor, ZC_PackColorUCharToUInt_RGB(255, 255, 255) }),
            // : ZC_GUI_ButtonBase(ZC_GUI_ObjData(0.f, 0.f, _color_default, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, color_under_cursor, ZC_PackColorUCharToUInt(255, 255, 255)),
            ZC_GUI_ButtonMouseText(0.f, 0.f, ZC_GUI_BF__None,                                                                                           //  color black
                ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(0.f, ZC_GUI_TFB_Indent::Center), wstr_axis, true, 0, ZC_GUI_TextAlignment::Left, 0)),
            axis(_axis),
            start_pos(_start_pos)
        {}

        void SetDepth(float depth)
        {       //  set depth for both objects of the button
            this->pObjData->depth = depth;
            this->textForButton.pObjData->depth = depth;
        }

        // void VCursorCollisionStart_Obj(float time) override
        // {
        //     if (this->pObjData->color == button_color_pressed) return;     //  button pressed, wait while up
        //     this->pObjData->color = color_under_cursor;
        //     VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
        // }

        // void VCursorCollisionEnd_Obj(float time) override
        // {
        //     if (this->pObjData->color == button_color_pressed) return;     //  button pressed, wait while up
        //     this->pObjData->color = color_default;
        //     VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
        // }

        void VMouseButtonLeftUp_Obj(float time) override
        {
            if (this->bs_mouseButton == BS_Pressed)
            {
                if (CheckCursorCollision_Obj())
                {       //  axis became not active, set color on defalut, in ZCR_Scene::GetScene()->SetAxise(axis) will calls ZCR_GUI_Orientation3D::UpdateAxisPositions() and update data on gpu
                    ZCR_Scene::GetScene()->SetAxise(axis);
                    pActiveAxis = this;     //  must be after ZCR_Scene::GetScene()->SetAxise(axis) to avoid unset pActiveAxis
                }
                else
                {
                    this->pObjData->color = this->colorsButton.color_button;
                    VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
                }
            }
            this->bs_mouseButton = BS_Released;
        }
    };

    static inline const float win_size = 100.f;
    ZC_Mat4<float> persp;
    ZC_EC ecCamChangePos;
    AxisButton axisButtons[6];

    ZCR_GUI_Orientation3D()
        : ZC_GUI_WinImmutable(ZC_WOIData(win_size, win_size, 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel), ZC_GUI_WF__NeedDraw | ZC_GUI_WF__NoBackground | ZC_GUI_WF__Stacionar),
        persp(ZC_Mat::Perspective(45.f, win_size / win_size, 0.1f, 100.f)),
        ecCamChangePos(ZC_Events::ConnectActiveCameraChangePosition({ &ZCR_GUI_Orientation3D::CameraChangePosition, this })),
        axisButtons
            {
                AxisButton(L"+X", ZCR_Axis::ZCR_A_PositiveX, { 1.f, 0.f, 0.f }, ZC_PackColorUCharToUInt_RGB(255, 0, 0), ZC_PackColorUCharToUInt_RGB(255, 60, 60)),
                AxisButton(L"-X", ZCR_Axis::ZCR_A_NegativeX, { -1.f, 0.f, 0.f }, ZC_PackColorUCharToUInt_RGB(255, 0, 0), ZC_PackColorUCharToUInt_RGB(255, 60, 60)),
                AxisButton(L"+Y", ZCR_Axis::ZCR_A_PositiveY, { 0.f, 1.f, 0.f }, ZC_PackColorUCharToUInt_RGB(0, 255, 0), ZC_PackColorUCharToUInt_RGB(60, 255, 60)),
                AxisButton(L"-Y", ZCR_Axis::ZCR_A_NegativeY, { 0.f, -1.f, 0.f }, ZC_PackColorUCharToUInt_RGB(0, 255, 0), ZC_PackColorUCharToUInt_RGB(60, 255, 60)),
                AxisButton(L"+Z", ZCR_Axis::ZCR_A_PositiveZ, { 0.f, 0.f, 1.f }, ZC_PackColorUCharToUInt_RGB(0, 0, 255), ZC_PackColorUCharToUInt_RGB(60, 60, 255)),
                AxisButton(L"-Z", ZCR_Axis::ZCR_A_NegativeZ, { 0.f, 0.f, -1.f }, ZC_PackColorUCharToUInt_RGB(0, 0, 255), ZC_PackColorUCharToUInt_RGB(60, 60, 255)),
            }
    {
        AxisButton::win_max_depth = (this->pObjData->depth - (depth_step_obj * 9.f));     //  window have 9 steps of depth
        for (AxisButton& ab : axisButtons) this->VAddObj_Obj(&ab);
    }

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
    {
        (*this->pBL) = _bl;
        *pBorder = ZC_GUI_Border{ .bl = _bl, .tr{ _bl[0] + this->VGetWidth_Obj(), _bl[1] + this->GetHeight() } };
        UpdateAxisPositions(false);
    }

    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override
    {
        if (!VCheckCursorCollision_Obj(x, y)) return false;
        
        rpObj = this;     //  top level, it's window
        
        AxisButton* pAB_collisioned = nullptr;
        for (AxisButton& axisButton : axisButtons)
        {
            if (axisButton.VCheckCursorCollision_Obj(x, y))
            {
                if (pAB_collisioned)    //  second axis under cursor
                {
                    if (axisButton.pObjData->depth < pAB_collisioned->pObjData->depth) pAB_collisioned = &axisButton;
                    break;  //  max two axis can be under cursor at the same time, it's second
                }
                else pAB_collisioned = &axisButton;
            }
        }

        if (pAB_collisioned) rpObj = pAB_collisioned;

        return true;
    }

    void CameraChangePosition(const ZC_Vec3<float>& camPos)
    {
        if (AxisButton::pActiveAxis)
        {
            AxisButton::pActiveAxis->pObjData->color = AxisButton::pActiveAxis->colorsButton.color_button;
            AxisButton::pActiveAxis = nullptr;
        }
        UpdateAxisPositions(true);
    }

    void UpdateAxisPositions(bool updateGPU)
    {       //  set orientation3D origin to some distance (3.3f) from cam pos to lookOn
            //  translate each buttonAxis to that origin from there start positions (get axises origin positions in 3d)
            //  multiply that positions on persp(our) and view(camera), and get [-1,1] screen coords, translate then to [0,1] and multiply with orientation3d window size (it origin coord in orientation3d window, in system widnow it somewhere in bl corner)
            //  origin minus button half width and button half height (get axises bl coords), and finaly translate that bl to orientation3d window bl.
        ZC_Camera* pActiveCamera = ZC_Camera::GetActiveCamera();
        ZC_Mat4<float> perspView = persp * (*pActiveCamera->GetView());

        ZC_Vec3<float> axises_origin = ZC_Vec::MoveByLength(pActiveCamera->GetPosition(), pActiveCamera->GetLookOn() - pActiveCamera->GetPosition(), 3.3f);        //  3.3 -> distance from camera to loockOn (orientation3D)
        
        AxisButton* ab_depths[6];
        int ab_depth_i = 0;

        for (AxisButton& axisButton : axisButtons)
        {
            ZC_Vec2<float> win_bl = this->Get_bl_Obj();

            ZC_Vec4<float> ab_origin_3d(axisButton.start_pos + axises_origin, 1.f);    //  move start position to axises origin
            ZC_Vec4<float> ab_origin_screenSpace = perspView * ab_origin_3d;
            
            float origin_x = ab_origin_screenSpace[0] / ab_origin_screenSpace[3];    //  [-1,1]
            origin_x = ZC_ToZeroOneRange(origin_x);     //  [0,1]
            origin_x *= win_size;   //  origin coord in orientation3d window, in system widnow it somewhere in bl corner
            origin_x += win_bl[0];  //  translate in system window to orientation3d window position
            float ab_bl_x = origin_x - axisButton.VGetWidth_Obj() / 2.f;   //  minus half button width to get button bl.x in system window

            float origin_y = ab_origin_screenSpace[1] / ab_origin_screenSpace[3];
            origin_y = ZC_ToZeroOneRange(origin_y);
            origin_y *= win_size;
            origin_y += win_bl[1];
            float ab_bl_y = origin_y - axisButton.GetHeight() / 2.f;   //  minus half button height to get button bl.y in system window

            axisButton.VSet_pBL_Obj({ ab_bl_x, ab_bl_y });
                //  depth
            axisButton.pObjData->depth = ab_origin_screenSpace[2];  //  no need to call AxisButton::SetDepth(), that temp depth values needed to sort axisbuttons and set actual depth throught AxisButton::SetDepth()
            ab_depths[ab_depth_i] = &axisButton;
            ++ab_depth_i;
        }
            //   set depth
        std::ranges::sort(ab_depths, [](AxisButton* a, AxisButton* b) { return a->pObjData->depth > b->pObjData->depth; });    //  ascending depth sort (first in ab_depths became farest)
        float cur_depth = this->pObjData->depth;    //  take window depth (farest obj)
        for (AxisButton* pAB : ab_depths)   //  setting depth beloning to the orientation3d window (farthest to closest)
        {
            pAB->SetDepth(cur_depth);
            cur_depth -= depth_step_obj;    //  next axis depth is closer
        }

        if (updateGPU)
        {
            this->VSubDataBL_Obj(axisButtons[0].Get_pBL_start(), axisButtons[5].VGet_pBL_end());    //  update bls
            this->VSubDataObjData_Obj(axisButtons[0].Get_pObjData_start(), axisButtons[5].VGet_pObjData_end());     //  update depths
        }
    }
};