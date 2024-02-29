#pragma once

#include <ZC_IGWindow.h>
#include <ZC/Tools/ZC_uptr.h>
#include "ZCR_IGWFigures.h"

class ZCR_IGWCreateAndMode : public ZC_IGWindow
{
public:
    ZCR_IGWCreateAndMode();

private:
    ZCR_IGWFigures winFigures;

    void DrawWindow() override;
};