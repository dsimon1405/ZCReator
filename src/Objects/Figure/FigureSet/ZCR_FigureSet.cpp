#include "ZCR_FigureSet.h"

ZCR_FigureSet::ZCR_FigureSet(ZC_sptr<ZC_DA<ZC_Quad>>&& _quads, ZC_sptr<ZC_DA<ZC_Triangle>>&& _triangles, ZC_sptr<ZC_DA<int>>&& _normals)
    : ZCR_VBO(std::move(_quads), std::move(_triangles), std::move(_normals))
{
    FillVBO();

    //  active points
    std::list<Points*> allActivePoints;
    for (auto& point : *spPoints) allActivePoints.emplace_back(&point);
    MakePointsActive(std::move(allActivePoints));

    typedef typename ZC_Uniform::Name UName;
    spRSADSPoint->SetUniformsData(UName::unModel, spMatModel->Begin());
    spRSADSLine->SetUniformsData(UName::unModel, spMatModel->Begin());
    spRSADSSurface->SetUniformsData(UName::unModel, spMatModel->Begin());

    // SwitchToSceneMode(ZCR_Scene::GetActiveSceneMode(), true);
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
        if (isActiveOnScene) SwitchGLElementOnRSLevel(RSLvl::Stencil, RSLvl::None, RSLvl::None);
        else SwitchGLElementOnRSLevel(RSLvl::Drawing, RSLvl::None, RSLvl::None);
        break;
    }
    case SceneMode::Edit:
    {
        if (isActiveOnScene) SwitchGLElementOnRSLevel(RSLvl::Drawing, RSLvl::Drawing, RSLvl::Drawing);
        else SwitchGLElementOnRSLevel(RSLvl::Drawing, RSLvl::None, RSLvl::None);
        break;
    }
    case SceneMode::Sculpting:
    {
        if (isActiveOnScene) SwitchGLElementOnRSLevel(RSLvl::Drawing, RSLvl::None, RSLvl::None);
        else SwitchGLElementOnRSLevel(RSLvl::None, RSLvl::None, RSLvl::None);
        break;
    }
    }
}

void ZCR_FigureSet::TranslateModelMatrix(const ZC_Vec3<float>& trans)
{
    spMatModel->Translate(trans);
}

void ZCR_FigureSet::SwitchGLElementOnRSLevel(RSLvl triangle, RSLvl point, RSLvl line)
{
    SwitchRSandDSTriangle(triangle);
    SwitchRSandDSPoint(point);
    SwitchRSandDSLine(line);
    // spRSADSs[GLElement::Triangle]->SwitchToLvl(triangle);
    // spRSADSs[GLElement::Point]->SwitchToLvl(point);
    // spRSADSs[GLElement::Line]->SwitchToLvl(line);
}