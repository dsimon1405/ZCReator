#pragma once

#include <ZC/Tools/ZC_uptr.h>

enum ZCR_SceneModes
{
    ZCR_SM_Model,
    ZCR_SM_Edit,
    ZCR_SM_Sculpting
};

// class ZCR_SceneMode
// {
// public:
//     ZCR_SceneMode();
//     virtual ~ZCR_SceneMode() = default;
    
//     virtual void DrawMode() = 0;

//     static ZC_uptr<ZCR_SceneMode> GetuptrOnMode(ZCR_SceneModes mode);
// };