#include "ZCR_GUI_W_Scene.h"

#include <ZC/GUI/ZC__GUI.h>
#include <Objects/Figure/ZCR_Figures.h>

ZCR_GUI_W_Scene::ZCR_GUI_W_Scene()
    : win(ZC_WOIData(300.f, ZC__GUI::GetFontHeight(), 0.f, ZC__GUI::GetFontHeight() * 4, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Top_Pixel), ZC_GUI_WF__NeedDraw | ZC_GUI_WF__Stacionar),
    cmt_scene(0.f, 0.f, L"Scene",  nullptr, { &ZCR_GUI_W_SceneOptions::OpenWindow, &w_sceneOptions }),
    dd_create(L"Create", { L"Cube" }, 0.f, 0.f, ZC_GUI_DDF__UnderCursor, { &ZCR_GUI_W_Scene::CreateFigure, this })
{
    win.AddRow(ZC_GUI_Row(ZC_GUI_RowParams(0.f, ZC_GUI_RowParams::Indent_X::Left, 0.f, 10.f),
        {
            cmt_scene.GetObj(),
            dd_create.GetObj(),
            mode.sdd_mode.GetObj()
        }));
}

void ZCR_GUI_W_Scene::CreateFigure(uint index)
{
    ZCR_Figures::CreateFigure(static_cast<ZCR_FigureName>(index));
}