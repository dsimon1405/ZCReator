#include "ZCR_Load.h"

#include <ZC/Objects/Text/ZC_Fonts.h>

void ZCR_LoadFonts()
{
    ZC_FontData fontDatas[]
    { 
        { ZC_F_ChunkFivePrint, 18 },    //  ZCR_Orientation3D
    };
    ZC_Fonts::Load(fontDatas, sizeof(fontDatas) / sizeof(ZC_FontData));
}