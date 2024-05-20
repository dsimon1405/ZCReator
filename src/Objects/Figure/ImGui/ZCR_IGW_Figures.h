#pragma once

#include <ZC_IGWindow.h>
#include <Objects/Figure/FigureData/ZCR_MatModel.h>
#include <ZC_IGTextCentered.h>
#include <ZC_IGInputText.h>

class ZCR_Figure;

class ZCR_IGW_Figures : public ZC_IGWindow, public ZC_IGInputText
{
public:
    ZCR_IGW_Figures(std::list<ZC_uptr<ZCR_Figure>>& _figures);

    void SetNameBuffer(ZCR_Figure* pFigure);

private:
    std::list<ZC_uptr<ZCR_Figure>>& rFigures;
    // char activeFigureNameBuffer[50];

    void VDrawWindowIGW() override;
    // void VInputCallbackIGIT(ZC_DA<char>& buffer) override;
    void VLostFocuseIGIT(ZC_DA<char>& buffer) override;

    void DrawFigures(ZCR_Figure* pActiveFigure);
    void NameChangeBufferEvent(ZC_DA<char>& buffer);
    void DrawScapeTransfomations(ZCR_Figure* pActiveFigure);
    void DrawAdditional(ZCR_Figure* pFigure);
};