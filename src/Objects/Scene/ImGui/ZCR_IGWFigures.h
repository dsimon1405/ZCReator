#pragma once

#include <ZC_IGWindow.h>

class ZCR_IGWFigures : public ZC_IGWindow
{
public:
    ZCR_IGWFigures();

private:
    void DrawWindow() override;
    void CreateFigure(int figureName);
};