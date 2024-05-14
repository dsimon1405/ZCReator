// #include "ZCR_ActiveFigures.h"

// #include <ZC/Tools/Container/ZC_ContFunc.h>

// void ZCR_ActiveFigures::Add(ZCR_Figure* pFigure)
// {
//     pFigure->ChangeSceneActivity(true);
//     activeFigures.emplace_back(pFigure);
// }

// void ZCR_ActiveFigures::Erase(ZCR_Figure* pFigure)
// {
//     pFigure->ChangeSceneActivity(false);
//     std::erase(activeFigures, pFigure);
// }

// void ZCR_ActiveFigures::Clear(bool changeActivityOnScene)
// {
//     if (changeActivityOnScene)
//         for (auto pFigure : activeFigures)
//             pFigure->ChangeSceneActivity(false);

//     activeFigures.clear();
// }

// bool ZCR_ActiveFigures::IsEmpty()
// {
//     return activeFigures.empty();
// }

// ZCR_Figure* ZCR_ActiveFigures::GetFirstAdded()
// {
//     return activeFigures.empty() ? nullptr : *(activeFigures.begin());
// }