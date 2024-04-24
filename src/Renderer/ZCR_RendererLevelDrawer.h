#pragma once

#include <ZC/Video/OpenGL/Renderer/ZC_RenderLevelDrawer.h>

struct ZCR_RendererLevelDrawer
{
private:
    static bool Init();

    static inline bool init = Init();

    static ZC_uptr<ZC_RenderLevelDrawer> GetRendererLevelDrawer(ZC_DrawLevel lvl);
};