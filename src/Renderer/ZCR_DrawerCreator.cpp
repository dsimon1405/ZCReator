#include "ZCR_RendererLevelDrawer.h"

#include "ZCR_RenderLevel.h"
#include <ZC/Video/OpenGL/Renderer/ZC_DrawerFL.h>

bool ZCR_DrawerCreator::Init()
{
    ZC_Drawer::SetUsersFunction_GetRendererLevelDrawer({ &(ZCR_DrawerCreator::GetRendererLevelDrawer) });
    return true;
}

ZC_uptr<ZC_Drawer> ZCR_DrawerCreator::GetRendererLevelDrawer(ZC_DrawerLevel lvl)
{
    switch (lvl)
    {
    case ZCR_DL_AlphaBlending: return 
        { new ZC_DrawerFL<const ZC_ShProg*, ZC_TexturesHolder, const ZC_VAO*, ZC_RLDData_Uniforms_GLDraw>(0, true, false, ZC_GLBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)) };
    default: assert(false); return nullptr;
    }
}