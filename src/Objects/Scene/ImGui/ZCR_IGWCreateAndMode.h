#pragma once

#include <ZC_IGWindow.h>
#include <ZC/Tools/ZC_uptr.h>
#include "ZCR_IGWFigures.h"
#include "ZCR_IGWScene.h"

class ZCR_IGWCreateAndMode : public ZC_IGWindow
{
public:
    ZCR_IGWCreateAndMode();

private:
    ZCR_IGWFigures igwFigures;
    ZCR_IGWScene igwScene;

    void VDrawWindowIGW() override;
};