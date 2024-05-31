#include "ZCR_ShaderKeeper.h"

ZCR_ShaderKeeper::ZCR_ShaderKeeper()
{
    std::vector<ZC_ShaderInput> v;
    if (ZCR_SV_All != 0) v.reserve(ZCR_SV_All);
    std::vector<ZC_ShaderInput> tc;
    if (ZCR_STC_All != 0) tc.reserve(ZCR_STC_All);
    std::vector<ZC_ShaderInput> te;
    if (ZCR_STE_All != 0) te.reserve(ZCR_STE_All);
    std::vector<ZC_ShaderInput> g;
    if (ZCR_SG_All != 0) g.reserve(ZCR_SG_All);
    std::vector<ZC_ShaderInput> f;
    if (ZCR_SF_All != 0) f.reserve(ZCR_SF_All);

    const ZC_FSPath shadersPath = ZC_FSPath(ZCR_ZCRDirPath).append("shaders");
    for (int i = ZCR_SV_None + 1; i < ZCR_SV_All; ++i) v.emplace_back(ZC_ShaderInput{ GetPath_Vertex(i, shadersPath), i });
    for (int i = ZCR_STC_None + 1; i < ZCR_STC_All; ++i) tc.emplace_back(ZC_ShaderInput{ GetPath_TessControl(i, shadersPath), i });
    for (int i = ZCR_STE_None + 1; i < ZCR_STE_All; ++i) te.emplace_back(ZC_ShaderInput{ GetPath_TessEvaluation(i, shadersPath), i });
    for (int i = ZCR_SG_None + 1; i < ZCR_SG_All; ++i) g.emplace_back(ZC_ShaderInput{ GetPath_Geometry(i, shadersPath), i });
    for (int i = ZCR_SF_None + 1; i < ZCR_SF_All; ++i) f.emplace_back(ZC_ShaderInput{ GetPath_Fragment(i, shadersPath), i });
    
    shaderManager.LoadShaderPrograms(v, tc, te, g, f);
}

std::string ZCR_ShaderKeeper::GetPath_Vertex(int shV, const ZC_FSPath& shadersPath)
{
    std::string path;
    switch (shV)
    {
    case ZCR_SV_colorFigure: return ZC_FSPath(shadersPath).append("colorFigure.vs").string();
    case ZCR_SV_point: return ZC_FSPath(shadersPath).append("point.vs").string();
    case ZCR_SV_lineFigure: return ZC_FSPath(shadersPath).append("lineFigure.vs").string();
    case ZCR_SV_stencilBorder: return ZC_FSPath(shadersPath).append("stencilBorder.vs").string();
    case ZCR_SV_texture_Vertex_TexCoord: return ZC_FSPath(shadersPath).append("texture_Vertex_TexCoord.vs").string();
    case ZCR_SV_lineMesh: return ZC_FSPath(shadersPath).append("lineMesh.vs").string();
    case ZCR_SV_textWindow: return ZC_FSPath(shadersPath).append("textWindow.vs").string();
    case ZCR_SV_textScene: return ZC_FSPath(shadersPath).append("textScene.vs").string();
    case ZCR_SV_textWindowIntoScene: return ZC_FSPath(shadersPath).append("textWindowIntoScene.vs").string();
    default: assert(false);
    }
    return path;
}

std::string ZCR_ShaderKeeper::GetPath_TessControl(int shTC, const ZC_FSPath& shadersPath)
{
    return {};
}

std::string ZCR_ShaderKeeper::GetPath_TessEvaluation(int shTE, const ZC_FSPath& shadersPath)
{
    return {};
}

std::string ZCR_ShaderKeeper::GetPath_Geometry(int shG, const ZC_FSPath& shadersPath)
{
    return {};
}

std::string ZCR_ShaderKeeper::GetPath_Fragment(int shF, const ZC_FSPath& shadersPath)
{
    switch (shF)
    {
    case ZCR_SF_color: return ZC_FSPath(shadersPath).append("color.fs").string();
    case ZCR_SF_colorTex: return ZC_FSPath(shadersPath).append("colorTex.fs").string();
    case ZCR_SF_text: return ZC_FSPath(shadersPath).append("text.fs").string();
    case ZCR_SF_orthoTexture: return ZC_FSPath(shadersPath).append("orthoTexture.fs").string();
    default: assert(false); return {};
    }
}