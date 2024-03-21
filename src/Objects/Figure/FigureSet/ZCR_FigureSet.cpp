#include "ZCR_FigureSet.h"

ZCR_FigureSet::ZCR_FigureSet(ZC_sptr<ZC_DA<ZC_Quad>>&& _quads, ZC_sptr<ZC_DA<ZC_Triangle>>&& _triangles, ZC_sptr<ZC_DA<int>>&& _normals)
    : ZCR_VBO(std::move(_quads), std::move(_triangles), std::move(_normals))
{
    FillVBO();

    //  active points
    std::list<Points*> allActivePoints;
    for (auto& point : *spPoints) allActivePoints.emplace_back(&point);
    MakePointsActive(std::move(allActivePoints));

    ZC_RSPDUniformData unModel(ZC_UN_unModel, spMatModel->Begin());
    rsControllerPoint.SetData(ZC_RSPDC_uniforms, &unModel);
    rsControllerLine.SetData(ZC_RSPDC_uniforms, &unModel);
    rsControllerSurface.SetData(ZC_RSPDC_uniforms, &unModel);
    // rsControllerPoint->SetUniformsData(UName::unModel, spMatModel->Begin());
    // rsControllerLine->SetUniformsData(UName::unModel, spMatModel->Begin());
    // rsControllerSurface->SetUniformsData(UName::unModel, spMatModel->Begin());

    spMatModel->Translate(4.f, 2.f, 1.f);
}

// ZCR_FigureSet::ZCR_FigureSet(ZCR_FigureSet&& tr)
//     : spVBO(std::move(tr.spVBO)),
//     spQuads(std::move(tr.spQuads)),
//     spTriangles(std::move(tr.spTriangles)),
//     spNormals(std::move(tr.spNormals)),
//     spTexCoords(std::move(tr.spTexCoords)),
//     colors(std::move(tr.colors)),
//     spMatModel(tr.spMatModel),
//     spPoints(std::move(tr.spPoints)),
//     activePoints(std::move(tr.activePoints)),
//     spRendererSets{ std::move(tr.spRendererSets[GLElement::Triangle]) },
//     spRSADSs{ std::move(tr.spRSADSs[GLElement::Triangle]) }
// {}

void ZCR_FigureSet::SwitchToSceneMode(SceneMode sceneMode, bool isActiveOnScene)
{
    switch (sceneMode)
    {
    case SceneMode::Model:
    {
        if (isActiveOnScene) SwitchGLElementOnRSLevel(ZC_RL_StencilBorder, ZC_RL_None, ZC_RL_None);
        else SwitchGLElementOnRSLevel(Drawing, ZC_RL_None, ZC_RL_None);
        break;
    }
    case SceneMode::Edit:
    {
        if (isActiveOnScene) SwitchGLElementOnRSLevel(Drawing, Drawing, Drawing);
        else SwitchGLElementOnRSLevel(Drawing, ZC_RL_None, ZC_RL_None);
        break;
    }
    case SceneMode::Sculpting:
    {
        if (isActiveOnScene) SwitchGLElementOnRSLevel(Drawing, ZC_RL_None, ZC_RL_None);
        else SwitchGLElementOnRSLevel(ZC_RL_None, ZC_RL_None, ZC_RL_None);
        break;
    }
    }
}

void ZCR_FigureSet::TranslateModelMatrix(const ZC_Vec3<float>& trans)
{
    spMatModel->Translate(trans);
}

void ZCR_FigureSet::SwitchGLElementOnRSLevel(ZC_RendererLevel triangle, ZC_RendererLevel point, ZC_RendererLevel line)
{
    SwitchRSandDSTriangle(triangle);
    SwitchRSandDSPoint(point);
    SwitchRSandDSLine(line);
    // spRSADSs[GLElement::Triangle]->SwitchToLvl(triangle);
    // spRSADSs[GLElement::Point]->SwitchToLvl(point);
    // spRSADSs[GLElement::Line]->SwitchToLvl(line);
}