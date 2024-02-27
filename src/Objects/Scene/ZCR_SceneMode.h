#pragma once

#include <ZC/Tools/ZC_uptr.h>

class ZCR_SceneMode
{
public:
    enum Mode
    {
        Model,
        Edit,
        Sculpting
    };

    ZCR_SceneMode();
    virtual ~ZCR_SceneMode() = default;
    
    virtual void DrawMode() = 0;

    static ZC_uptr<ZCR_SceneMode> GetuptrOnMode(Mode mode);
};