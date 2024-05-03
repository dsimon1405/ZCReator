#pragma once

#include <ZC_IGWindow.h>

class ZCR_IGWFigures : public ZC_IGWindow
{
public:
    ZCR_IGWFigures();

private:
    int creatingFigureName;

    void VDrawWindowIGW() override;
    void FigureCreation(int _creatingFigureName);
    void CreateFigure();
};