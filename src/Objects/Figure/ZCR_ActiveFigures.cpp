#include "ZCR_ActiveFigures.h"

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

void ZCR_ActiveFigures::Add(ZCR_Figure* pFigure)
{
    pFigure->ChangeSceneActivity(true);
    activeFigures.emplace_front(pFigure);
}

void ZCR_ActiveFigures::Erase(ZCR_Figure* pFigure)
{
    pFigure->ChangeSceneActivity(false);
    if (!ZC_ForwardListErase(activeFigures, pFigure))
        ZC_ErrorLogger::Err("Can't find ZCR_FigureCreator to erase from activeFigures!", __FILE__,__LINE__);
}

void ZCR_ActiveFigures::Clear(bool changeActivityOnScene)
{
    if (changeActivityOnScene)
        for (auto pFigure : activeFigures)
            pFigure->ChangeSceneActivity(false);

    activeFigures.clear();
}

bool ZCR_ActiveFigures::IsEmpty()
{
    return activeFigures.empty();
}
