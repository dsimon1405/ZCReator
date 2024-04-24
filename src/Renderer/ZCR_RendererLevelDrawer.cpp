#include "ZCR_RendererLevelDrawer.h"

#include "ZCR_FrameBuffer.h"
#include "ZCR_RLDAlphaBlending.h"

bool ZCR_RendererLevelDrawer::Init()
{
    ZC_RenderLevelDrawer::SetUsersFunction_GetRendererLevelDrawer({ &(ZCR_RendererLevelDrawer::GetRendererLevelDrawer) });
    return true;
}

ZC_uptr<ZC_RenderLevelDrawer> ZCR_RendererLevelDrawer::GetRendererLevelDrawer(ZC_DrawLevel lvl)
{
    switch (lvl)
    {
    case ZCR_DL_AlphaBlending: return { new ZCR_RLDAlphaBlending };
    default: assert(false); return nullptr;
    }
}