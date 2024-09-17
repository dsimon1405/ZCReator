#pragma once

#include <ZC/GUI/Button/ZC_GUI__Click.h>
#include <ZC/GUI/ZC_GUI__DropDown.h>
#include <Objects/Scene/GUI/Mode/ZCR_GUI_SceneMode.h>
#include "ZCR_GUI_W_SceneOptions.h"

struct ZCR_GUI_W_Scene
{
    ZCR_GUI_W_Scene();

private:
    ZC_GUI__WinImmutable win;
        //  Scene button
    ZC_GUI__ClickMouseText cmt_scene;
    ZCR_GUI_W_SceneOptions w_sceneOptions;
        //  Create drop down
    ZC_GUI__DropDown dd_create;
        //  Mode switch drop down
    ZCR_GUI_SceneMode mode;

        //  dd_create callback
    void CreateFigure(uint index);
};