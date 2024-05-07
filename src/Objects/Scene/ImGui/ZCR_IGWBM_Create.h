#pragma once

#include <ZC_IGWBM.h>

class ZCR_IGWBM_Create : public ZC_IGWBM
{
public:
    ZCR_IGWBM_Create();

private:
    int creatingFigureName;

    void VDrawWindowIGW() override;
    void VCallButtonDownBM(ZC_ButtonID buttonId, float time) override;  //  here heir must realize switch for all necessary methods

    void FigureCreation(int _creatingFigureName, bool createNow);
    void CreateFigure();
};