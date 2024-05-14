#pragma once

#include <ZC_IGWindow.h>
#include <Objects/Figure/FigureData/ZCR_MatModel.h>

class ZCR_Figure;

class ZCR_IGW_Figures : public ZC_IGWindow
{
public:
    ZCR_IGW_Figures(std::list<ZC_uptr<ZCR_Figure>>& _figures);

private:
    std::list<ZC_uptr<ZCR_Figure>>& figures;

    void VDrawWindowIGW() override;

    void DrawFigures(ZCR_Figure* pActiveFigure);
    //  Draw translate, rotate, scael. buttonId - specifies different id for different buttons with same name.
    void DrawTRS(char buttonId, float step, const char* name, ZCR_MatModel& rMatModel, float(ZCR_MatModel::*pFuncGet)(ZCR_MatModelAxis),
        void(ZCR_MatModel::* pFuncSet)(ZCR_MatModelAxis, float, bool));
    void DrawAdditional(ZCR_Figure* pFigure);
};