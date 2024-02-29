#pragma once

#include <ZC_IGWindow.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>

class ZCR_IGWFigures : public ZC_IGWindow
{
public:
    ZCR_IGWFigures();

private:
    ZC_SConnection sconHandleEventsEnd; //  put here creating Figure function (to avoid opengl conflict)
    int creatingFigureName;

    void DrawWindow() override;
    void CreateFigure(int _creatingFigureName);
    void Callback();
};