#include <ZC/main/ZC_main.h>
#include <ZC/Video/ZC_Window.h>

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAOs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <ZC/Video/OpenGL/Buffer/ZC_Buffers.h>
#include <ZC/Objects/Camera/ZC_Camera.h>

#include <ZC/Tools/Time/ZC_Clock.h>

#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/Tools/Console/ZC_cout.h>

#include <ZC/Events/ZC_Events.h>

#include <ZC/Objects/ZC_Object.h>

#include <ZC/Tools/Math/ZC_Figures.h>

#include <ZC/Video/OpenGL/Texture/ZC_Textures.h>

// #include <Objects/ImGui/ZCR_ImGui.h>
#include <Objects/Scene/ZCR_Scene.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>


struct NewSquare
{
    ZC_Quad bl;
    ZC_Quad tl;
    ZC_Quad tr;
    ZC_Quad br;
    void SetCenter(const ZC_Vec3<float>& center) { bl.tr = tl.br = tr.bl = br.tl = center; }
    ZC_Vec3<float>& GetCenter() { return bl.tr; }
    void SetEdgePointL(const ZC_Vec3<float>& point) { bl.tl = tl.bl = point; }
    void SetEdgePointT(const ZC_Vec3<float>& point) { tl.tr = tr.tl = point; }
    void SetEdgePointR(const ZC_Vec3<float>& point) { tr.br = br.tr = point; }
    void SetEdgePointB(const ZC_Vec3<float>& point) { bl.br = br.bl = point; }
};

struct Square
{
    ZC_Vec3<float> edgeCenterL,
        edgeCenterT,
        edgeCenterR,
        edgeCenterB;
    bool isEdgePointL = false,  //  is left edge point calculated
        isEdgePointT = false,
        isEdgePointR = false,
        isEdgePointB = false;
};

struct VertexData
{
    NewSquare& rNS;
    ZC_Vec3<float>& rNewPoint,
        & rEC1,
        & rEC2,
        & rCorner1,
        & rCorner2;
    void(NewSquare::*pSetEP1)(const ZC_Vec3<float>&);
    void(NewSquare::*pSetEP2)(const ZC_Vec3<float>&);
    bool& rIsEP1,
        rIsEP2;

    VertexData(NewSquare& _rNewSquare,
                ZC_Vec3<float>& _rNewPoint,
                ZC_Vec3<float>& _rEdgeCenter1,
                ZC_Vec3<float>& _rEdgeCenter2,
                ZC_Vec3<float>& _rCorner1,
                ZC_Vec3<float>& _rCorner2,
                void(NewSquare::*_pSetEdgePoint1)(const ZC_Vec3<float>&),
                void(NewSquare::*_pSetEdgePoint2)(const ZC_Vec3<float>&),
                bool& _rIsEdgePoint1,
                bool& _rIsEdgePoint2)
    : rNS(_rNewSquare),
    rNewPoint(_rNewPoint),
    rEC1(_rEdgeCenter1),
    rEC2(_rEdgeCenter2),
    rCorner1(_rCorner1),
    rCorner2(_rCorner2),
    pSetEP1(_pSetEdgePoint1),
    pSetEP2(_pSetEdgePoint2),
    rIsEP1(_rIsEdgePoint1),
    rIsEP2(_rIsEdgePoint2)
    {}
};

struct CC
{
    struct Vertex
    {
        ZC_Vec3<float> vertex;
        std::list<VertexData> vdList;

        bool operator == (const ZC_Vec3<float>& _v) const noexcept
        {
            return vertex == _v;
        }
    };

    ZC_DA<Square> squares;
    NewSquare* newSquares;
    size_t squaresCount;
    std::list<Vertex> vertices;

    CC(ZC_DA<ZC_Quad>& quads)
        : squares(quads.size),
        newSquares(new NewSquare[quads.size])
    {
        for (size_t _squaresIndex = 0; _squaresIndex < quads.size; ++_squaresIndex)
        {
            Square& rSSq = squares[_squaresIndex];
            ZC_Quad* pQSq = quads.pHead;
            rSSq.edgeCenterL = (pQSq->bl + pQSq->tl) / 2.f;
            rSSq.edgeCenterT = (pQSq->tl + pQSq->tr) / 2.f;
            rSSq.edgeCenterR = (pQSq->tr + pQSq->br) / 2.f;
            rSSq.edgeCenterB = (pQSq->br + pQSq->bl) / 2.f;
            NewSquare& rNS = newSquares[_squaresIndex];
            rNS.SetCenter(quads.pHead->Center());
            Add(pQSq->bl, VertexData(rNS, rNS.bl.bl, rSSq.edgeCenterB, rSSq.edgeCenterL, pQSq->br, pQSq->tl,
                &NewSquare::SetEdgePointB, &NewSquare::SetEdgePointL, rSSq.isEdgePointB, rSSq.isEdgePointL));
            Add(pQSq->tl, VertexData(rNS, rNS.tl.tl, rSSq.edgeCenterT, rSSq.edgeCenterL, pQSq->tr, pQSq->bl,
                &NewSquare::SetEdgePointT, &NewSquare::SetEdgePointL, rSSq.isEdgePointT, rSSq.isEdgePointL));
            Add(pQSq->tr, VertexData(rNS, rNS.tr.tr, rSSq.edgeCenterT, rSSq.edgeCenterR, pQSq->tl, pQSq->br,
                &NewSquare::SetEdgePointT, &NewSquare::SetEdgePointR, rSSq.isEdgePointT, rSSq.isEdgePointR));
            Add(pQSq->br, VertexData(rNS, rNS.br.br, rSSq.edgeCenterB, rSSq.edgeCenterR, pQSq->bl, pQSq->tr,
                &NewSquare::SetEdgePointB, &NewSquare::SetEdgePointR, rSSq.isEdgePointB, rSSq.isEdgePointR));
        }
    }
    
    void Add(const ZC_Vec3<float>& vertex, const VertexData& vData)
    {
        auto pVertex = ZC_Find(vertices, vertex);
        if (pVertex) pVertex->vdList.emplace_back(vData);
        else vertices.emplace_back(Vertex{vertex, {vData}});
    }

    //  Catmull Clark https://en.wikipedia.org/wiki/Catmull%E2%80%93Clark_subdivision_surface
    ZC_DA<ZC_Quad> Calculate()
    {
        for (auto& vert : vertices)
        {
            size_t faces = vert.vdList.size();
            auto vdI = vert.vdList.begin();
            if (faces == 1)   //  point alone
            {
                //  set edges points as edges center
                (vdI->rNS.*vdI->pSetEP1)(vdI->rEC1);
                (vdI->rNS.*vdI->pSetEP2)(vdI->rEC2);
                //  move the center in the direction of the corner + edge centers, by an average length to the center of each edge
                //  (If the figure is a square, the result will be movement by the radius of the inscribed circle)
                vdI->rNewPoint = ZC_Vec::MoveByLength(vdI->rNS.GetCenter(), (vert.vertex + vdI->rEC1 + vdI->rEC2) / 3.f - vdI->rNS.GetCenter(),
                    (ZC_Vec::Length(vdI->rNS.GetCenter() - vdI->rEC1) + ZC_Vec::Length(vdI->rNS.GetCenter() - vdI->rEC2)) / 2.f);
                continue;
            }
    
            ZC_Vec3<float> centersSum,
                commonEdgesCentersSum,
                singleEgdesCentersSum;
            unsigned short commonEdges = 0;
            unsigned short singleEdges = 0;
            //  find edges points
            for (; vdI != vert.vdList.end(); ++vdI)
            {
                centersSum += vdI->rNS.GetCenter();
                for (auto vdI2 = vdI; vdI2 != vert.vdList.end() && (!vdI->rIsEP1 || !vdI->rIsEP2); ++vdI2)
                {
                    if (!vdI->rIsEP1 && !vdI2->rIsEP1 && vdI->rEC1 == vdI2->rEC1)   //  common edge, calculate edge point
                        CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC1, (vert.vertex + vdI->rCorner1 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
                            vdI->rIsEP1, vdI2->rIsEP1, vdI->rNS, vdI2->rNS, vdI->pSetEP1, vdI2->pSetEP1, commonEdges);
                    if (!vdI->rIsEP1 && !vdI2->rIsEP2 && vdI->rEC1 == vdI2->rEC2)
                        CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC1, (vert.vertex + vdI->rCorner1 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
                            vdI->rIsEP1, vdI2->rIsEP2, vdI->rNS, vdI2->rNS, vdI->pSetEP1, vdI2->pSetEP2, commonEdges);
                    if (!vdI->rIsEP2 && !vdI2->rIsEP1 && vdI->rEC2 == vdI2->rEC1)
                        CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC2, (vert.vertex + vdI->rCorner2 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
                            vdI->rIsEP2, vdI2->rIsEP1, vdI->rNS, vdI2->rNS, vdI->pSetEP2, vdI2->pSetEP1, commonEdges);
                    if (!vdI->rIsEP2 && !vdI2->rIsEP2 && vdI->rEC2 == vdI2->rEC2)
                        CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC2, (vert.vertex + vdI->rCorner2 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
                            vdI->rIsEP2, vdI2->rIsEP2, vdI->rNS, vdI2->rNS, vdI->pSetEP2, vdI2->pSetEP2, commonEdges);
                }  //  no common edges, set edge point as edge center
                if (!vdI->rIsEP1) SingleEdgeCenter(vdI->rEC1, vdI->rIsEP1, vdI->rNS, vdI->pSetEP1, singleEgdesCentersSum, singleEdges);
                if (!vdI->rIsEP2) SingleEdgeCenter(vdI->rEC2, vdI->rIsEP2, vdI->rNS, vdI->pSetEP2, singleEgdesCentersSum, singleEdges);
            }

            ZC_Vec3<float> newPoint;
            if (singleEdges == 0) newPoint = ((centersSum / faces) + (commonEdgesCentersSum / commonEdges) * 2.f + vert.vertex * (commonEdges - 3.f)) / commonEdges;  //  barycenter
            else if (singleEdges == 2) newPoint = (singleEgdesCentersSum + vert.vertex * 2.f) / 4.f;
            else newPoint = vert.vertex;

            for (auto& rvd : vert.vdList) rvd.rNewPoint = newPoint;
        }

        ZC_DA<ZC_Quad> result(reinterpret_cast<ZC_Quad*>(newSquares), squares.size * 4);
        squares.pHead = nullptr;
        return result;
    }

    void SingleEdgeCenter(const ZC_Vec3<float>& point, bool& rIsEP, NewSquare& rNS, void(NewSquare::*pSet)(const ZC_Vec3<float>&),
        ZC_Vec3<float>& rSECS, unsigned short& rSingleEdges)
    {
        rIsEP = true;
        (rNS.*pSet)(point);
        rSECS += point;
        ++rSingleEdges;
    }

    void CommonEdgeCenter(ZC_Vec3<float>& rCECSum, ZC_Vec3<float>& rCEC, const ZC_Vec3<float>& point, bool& rIsEP1, bool& rIsEP2, NewSquare& rNS1, NewSquare& rNS2,
        void(NewSquare::*pSet1)(const ZC_Vec3<float>&), void(NewSquare::*pSet2)(const ZC_Vec3<float>&), unsigned short& rCommonEdges)
    {
        rCECSum += rCEC;
        rIsEP1 = true;
        (rNS1.*pSet1)(point);
        rIsEP1 = true;
        (rNS2.*pSet2)(point);
        ++rCommonEdges;
    }
};
//  ZCR_Camera scroll and IGWindow for axis

// struct ZC_RSTexturePointer : public ZC_RS
// {
//     ZC_Texture* pTexture;

//     ZC_RSTexturePointer(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
//             std::forward_list<ZC_Buffer>&& _buffers, ZC_Texture* _pTexture);
//     ZC_RSTexturePointer(ZC_RSTexturePointer&& rs);

//     void Draw(ZC_RendererLevel lvl) override;

//     /*
//     Return unique pointer to ZC_RendererSetAndDrawingSet.

//     texSetName - name of the texture set (ZC_RSTexs::TexSet), can be nullptr if used for heir ZC_RSNonTex.
//     stencilScale - scale for drawing the stencil border of the object. Should be greater than 1.0f. Can be anything if ZC_RendererSet::Level::Stencil will not be used.
//     stencilColor - color of stencil border packed in unsigned int. Packing -> unsigned char[32] -> [0 - 7]{no metter}, [7 - 15]{red}, [15 - 23]{green}, [23 - 31]{blue}
//     */
//     ZC_uptr<ZC_RendererSetAndDrawingSet> Make_uptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor) override;

//     /*
//     Return shared pointer to ZC_RendererSetAndDrawingSet.

//     texSetName - name of the texture set (ZC_RSTexs::TexSet), can be nullptr if used for heir ZC_RSNonTex.
//     stencilScale - scale for drawing the stencil border of the object. Should be greater than 1.0f. Can be anything if ZC_RendererSet::Level::Stencil will not be used.
//     stencilColor - color of stencil border packed in unsigned int. Packing -> unsigned char[32] -> [0 - 7]{no metter}, [7 - 15]{red}, [15 - 23]{green}, [23 - 31]{blue}
//     */
//     ZC_sptr<ZC_RendererSetAndDrawingSet> Make_sptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor) override;
//     void Add(ZC_DrawingSet* pDS) override;
//     void Erase(ZC_DrawingSet* pDS) override;
    
// private:
//     LevelController levelController;
// };

// ZC_RSTexturePointer::ZC_RSTexturePointer(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
//         std::forward_list<ZC_Buffer>&& _buffers, ZC_Texture* _pTexture)
//     : ZC_RS(pShPInitSet, std::move(_vao), std::move(_upDraw), std::move(_buffers)),
//     pTexture(_pTexture)
// {}

// ZC_RSTexturePointer::ZC_RSTexturePointer(ZC_RSTexturePointer&& rs)
//     : ZC_RS(dynamic_cast<ZC_RS&&>(rs))
// {}

// void ZC_RSTexturePointer::Draw(ZC_RendererLevel lvl)
// {
//     vao.BindVertexArray();
//     levelController.Draw(lvl, upGLDraw, pTexture, 1);
// }

// ZC_uptr<ZC_RendererSetAndDrawingSet> ZC_RSTexturePointer::Make_uptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
// {
//     return { new ZC_RendererSetAndDrawingSet(this, { pBaseUniforms->GetCopy(), ZC_RendererLevel::ZC_RL_None, nullptr, stencilScale, stencilColor }) };
// }

// ZC_sptr<ZC_RendererSetAndDrawingSet> ZC_RSTexturePointer::Make_sptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor)
// {
//     return { new ZC_RendererSetAndDrawingSet(this, { pBaseUniforms->GetCopy(), ZC_RendererLevel::ZC_RL_None, nullptr, stencilScale, stencilColor }) };
// }

// void ZC_RSTexturePointer::Add(ZC_DrawingSet* pDS)
// {
//     if (levelController.Add(pDS)) AddToRenderer(pDS->lvl);
// }

// void ZC_RSTexturePointer::Erase(ZC_DrawingSet* pDS)
// {
//     if (levelController.Erase(pDS)) EraseFromRenderer(pDS->lvl);
// }
#include <ZC/Tools/Math/ZC_Vec2.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Video/OpenGL/ZC_OpenGL.h>

#include <ZC/File/ZC_File.h>
#include <ZC/Tools/Math/ZC_Math.h>

enum ZC_TextAlignment
{
    Left,
    Right,
    Center,
};

enum ZC_FontName
{
    Arial,
};

struct ZC_Fonts
{
    struct NameHeight
    {
        ZC_FontName name;
        size_t height;

        bool operator < (const NameHeight& fs) const noexcept
        {
            return name < fs.name || height < fs.height;    //  && || ???
        }
    };

    struct Character
    {
        float advX; 	// advance.x
        float advY;	    // advance.y

        float bitmapW;	    // bitmap.width;
        float bitmapH;  	// bitmap.height;

        float bitmapLeft;	// bitmap_left;
        float bitmapTop;	// bitmap_top;

        float texX_left;	    // x offset of glyph in texture coordinates
        float texY_bottom;	    // y offset of glyph in texture coordinates
        float texX_right;	    // x offset of glyph in texture coordinates
        float texY_top;	        // y offset of glyph in texture coordinates
    };

    struct Font
    {
        struct Point
        {
            float x,
                y;
            ushort s,  // texCoords X
                t;  // texCoords Y
        };

        ZC_Texture texture;
        std::map<char, Character> characters;

        struct LineData
        {
            size_t coordsStartIndex,
                coordsCount;
            float mustBeUp_Y,
                width;
        };

        //  makeOriginCenter - make origin center of text (by default origin bottom left for indent translates in ZC_TextWindow)
        std::vector<Point> FillCoords(const std::string& text, bool makeOriginCenter, ZC_TextAlignment alignment, float& rTextWidth, float& rTextHeight)
        {
            auto newLineCount = std::count(text.begin(), text.end(), '\n');
            std::vector<LineData> linesData;
            linesData.reserve(newLineCount + 1);   //  + 1 -> minimum 1 line

            std::vector<Point> coords;
            coords.reserve((text.length() - std::count(text.begin(), text.end(), ' ') - newLineCount) * 4); //  4 -> vertices count, except ' ' and '\n'

            FillCoordsAndLines(text, coords, linesData, rTextWidth, rTextHeight);

            makeOriginCenter ? CalcutatePositionXYWithOriginCenter(linesData, coords, alignment, rTextWidth, rTextHeight)
                : CalcutatePositionXY(linesData, coords, alignment, rTextWidth);

            return coords;
        }

        void FillCoordsAndLines(const std::string& text, std::vector<Point>& rCoords, std::vector<LineData>& rLinesData, float& rTotalWidth, float& rTotalHeight)
        {
            float startX = 0,
                startY = 0,
                lineHeightWithoughtBottomTail = 0,
                lineHeightBottomTail = 0;
            size_t coordsStartIndex = 0,
                coordsCount = 0;
            for (auto c : text)
            {
                if (c == '\n')
                {
                    AddLine(lineHeightBottomTail, lineHeightWithoughtBottomTail, rLinesData, coordsStartIndex, coordsCount, startX, rTotalWidth, rTotalHeight);
                    startX = 0;
                    startY = 0;
                    lineHeightWithoughtBottomTail = 0;
                    lineHeightBottomTail = 0;
                    coordsCount = 0; 
                    continue;
                }

                const Character& ch = characters[c];
                float x = startX + ch.bitmapLeft,
                    y = startY + (ch.bitmapH - (ch.bitmapH - ch.bitmapTop)),
                    w = ch.bitmapW,
                    h = ch.bitmapH;

                startX += ch.advX;
                // startY += ch.advY;

                float mustBeUp = h - y;
                if (mustBeUp > lineHeightBottomTail) lineHeightBottomTail = mustBeUp;

                float symbolsHeightWithoughtDownTail = h - mustBeUp;
                if (lineHeightWithoughtBottomTail < symbolsHeightWithoughtDownTail) lineHeightWithoughtBottomTail = symbolsHeightWithoughtDownTail;

                if (!w || !h) continue;   //  whitespace

                ushort texCoord_l = ZC_PackTexCoordFloatToUShort(ch.texX_left);
                ushort texCoord_r = ZC_PackTexCoordFloatToUShort(ch.texX_right);
                ushort texCoord_t = ZC_PackTexCoordFloatToUShort(ch.texY_top);
                ushort texCoord_b = ZC_PackTexCoordFloatToUShort(ch.texY_bottom);

                rCoords.emplace_back(Point{ x, y, texCoord_l, texCoord_b });            //  bl  full texture of symbols is Y inversed!
                rCoords.emplace_back(Point{ x + w, y - h, texCoord_r, texCoord_t });    //  tr  full texture of symbols is Y inversed!
                rCoords.emplace_back(Point{ x, y - h, texCoord_l, texCoord_t });        //  tl  full texture of symbols is Y inversed!
                rCoords.emplace_back(Point{ x + w, y, texCoord_r, texCoord_b });        //  br  full texture of symbols is Y inversed!

                coordsCount += 4;
            }

            AddLine(lineHeightBottomTail, lineHeightWithoughtBottomTail, rLinesData, coordsStartIndex, coordsCount, startX, rTotalWidth, rTotalHeight);
        }

        void CalcutatePositionXY(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth)
        {
            for (auto& lineData : rLinesData)    // Y alignment (down tails and next lines height)
            {
                if (lineData.mustBeUp_Y == 0) continue;
                for (size_t i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
                    rCoords[i].y += lineData.mustBeUp_Y;
            }

            if (alignment == Left) return;

            for (auto& lineData : rLinesData)    //  X alignment
            {
                if (lineData.width == textWidth) continue;
                float offsetX = alignment == Right ? textWidth - lineData.width : (textWidth - lineData.width) / 2.f;  //  otherwise Alignment_Center, Alignment_Left is default
                for (size_t i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
                    rCoords[i].x += offsetX;                
            }
        }

        void CalcutatePositionXYWithOriginCenter(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords,
            ZC_TextAlignment alignment, float rTextWidth, float rTextHeight)
        {
            float halfTextHeight = rTextHeight / 2.f;
            for (auto& lineData : rLinesData)    // Y alignment (down tails and next lines height)
            {
                for (size_t i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
                    rCoords[i].y += lineData.mustBeUp_Y - halfTextHeight;    //  - halfTextHeight (make texts Y origin center)
            }

            float halfTextWidth = rTextWidth / 2.f;
            for (auto& lineData : rLinesData)    //  X alignment
            {
                float offsetX = -halfTextWidth;     //  make texts X origin center
                if (lineData.width != rTextWidth)
                    offsetX += alignment == Right ? rTextWidth - lineData.width : (rTextWidth - lineData.width) / 2.f;  //  otherwise Alignment_Center, Alignment_Left is default
                for (size_t i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
                    rCoords[i].x += offsetX;                
            }
        }

        void AddLine(float lineHeightDownTail, float lineHeightWithoughtButtomTail, std::vector<LineData>& rLinesData, size_t& rCoordsStartIndex,
            size_t coordsCount, float startX, float& rTotalWidth, float& rTotalHeight)
        {
            float previousLinesMustBeUp_Y = lineHeightDownTail + lineHeightWithoughtButtomTail;
            for (auto& lineData : rLinesData) lineData.mustBeUp_Y += previousLinesMustBeUp_Y;
            
            rLinesData.emplace_back(LineData{ rCoordsStartIndex, coordsCount, lineHeightDownTail, startX });    //  last startX same as line width
            if (startX > rTotalWidth) rTotalWidth = startX;
            rCoordsStartIndex += coordsCount;

            rTotalHeight += previousLinesMustBeUp_Y;
        }
    };

    static inline std::map<NameHeight, Font> fonts;
    static const uint texMaxW = 1024,
        borderPixel = 1;
    static const ulong firstASCII = 32,
        lastASCII = 127;

    static void Load(NameHeight* names, size_t namesCount)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) ZC_ErrorLogger::Err("Fail FT_Init_FreeType()!", __FILE__, __LINE__);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (size_t namesIndex = 0; namesIndex < namesCount; ++namesIndex)
        {
            auto fontsIter = fonts.find(names[namesIndex]);
            if (fontsIter != fonts.end()) continue;

            FT_Face face;
            if (FT_New_Face(ft, GetPath(names[namesIndex].name).c_str(), 0, &face))
            {
                ZC_ErrorLogger::Err("Fail FT_New_Face()!", __FILE__, __LINE__);
                continue;
            }

            if (FT_Set_Pixel_Sizes(face, 0, names[namesIndex].height))
            {
                ZC_ErrorLogger::Err("Fail FT_Set_Pixel_Sizes()!", __FILE__, __LINE__);
                continue;
            }

            fonts.emplace(names[namesIndex], MakeFont(face));
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    static Font* GetFont(NameHeight name)
    {
        auto fontsIter = fonts.find(name);
        if (fontsIter == fonts.end()) ZC_ErrorLogger::Err("Error in GetFont() Font wasn't loaded!", __FILE__,__LINE__);
        return &(fontsIter->second);
    }

    static std::string GetPath(ZC_FontName name)
    {
        static const ZC_FSPath ZC_fontsPath = ZC_FSPath(ZC_ZCDirPath).append("fonts");
        switch (name)
        {
        case Arial: return ZC_FSPath(ZC_fontsPath).append("arial.ttf").string();
        default: return "";
        }
    }

    static Font MakeFont(const FT_Face& face)
    {
        uint texW = 0,
            texH = 0;
        CalculateTextureSize(texW, texH, face);

        ZC_Texture texture(GL_TEXTURE_2D, GL_RED, texW, texH, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
        texture.Bind();

        float texX = 0,
            texY = 0;
        unsigned rowH = 0;
        FT_GlyphSlot glyph = face->glyph;

        std::map<char, Character> characters;
    
        for (auto symbol = firstASCII; symbol < lastASCII; ++symbol)
        {
            if (FT_Load_Char(face, symbol, FT_LOAD_RENDER)) continue;

            if (texX + glyph->bitmap.width + borderPixel > texMaxW)
            {
                texY += rowH + borderPixel;
                rowH = 0;
                texX = 0;
            }
            
            if (glyph->bitmap.width != 0 && glyph->bitmap.rows != 0)
                glTexSubImage2D(GL_TEXTURE_2D, 0, texX, texY, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
                
            float texX_left = texX / static_cast<float>(texW),
                texY_bottom = texY / static_cast<float>(texH);
            characters.emplace(symbol, Character
                {
                    static_cast<float>(glyph->advance.x >> 6),
                    static_cast<float>(glyph->advance.y >> 6),
                    static_cast<float>(glyph->bitmap.width),
                    static_cast<float>(glyph->bitmap.rows),
                    static_cast<float>(glyph->bitmap_left),
                    static_cast<float>(glyph->bitmap_top),
                    texX_left,
                    texY_bottom,
                    texX_left + (static_cast<float>(glyph->bitmap.width) / texW),
                    texY_bottom + (static_cast<float>(glyph->bitmap.rows) / texH)
                });

            texX += glyph->bitmap.width + borderPixel;
            rowH = std::max(glyph->bitmap.rows, rowH);
        }
        texture.Unbind();

        return { std::move(texture), std::move(characters) };
    }

    static void CalculateTextureSize(uint& texW, uint& texH, const FT_Face& face)
    {
        unsigned rowW = 0,
            rowH = 0;
        FT_GlyphSlot glyph = face->glyph;

        for (auto symbol = firstASCII; symbol < lastASCII; ++symbol)
        {
            if (FT_Load_Char(face, symbol, FT_LOAD_RENDER))
            {
                ZC_ErrorLogger::Err("Fail FT_Load_Char()!", __FILE__, __LINE__);
                continue;
            }

            if (rowW + glyph->bitmap.width + borderPixel > texMaxW)
            {
                texW = std::max(rowW, texW);
                texH += rowH + borderPixel;
                rowW = 0;
                rowH = 0;
            }

            rowW += glyph->bitmap.width + borderPixel;
            rowH = std::max(rowH, glyph->bitmap.rows);
        }

        texW = std::max(texW, rowW);
        texH += rowH + borderPixel;
    }
};



template<typename TpHead>
void FillTriangleElements(TpHead pElementsHead, size_t elementsSize, size_t quadsElementsCount)
{
    size_t elementsI = 0,
        index = 0;
    for ( ; elementsI < quadsElementsCount; elementsI += 6)
    {
        pElementsHead[elementsI] = pElementsHead[elementsI + 3] = index++;
        pElementsHead[elementsI + 1] = pElementsHead[elementsI + 5] = index++;
        pElementsHead[elementsI + 2] = index++;
        pElementsHead[elementsI + 4] = index++;
    }
    while (elementsI < elementsSize)
    {
        pElementsHead[elementsI++] = index++;
        pElementsHead[elementsI++] = index++;
        pElementsHead[elementsI++] = index++;
    }
}

/*
Create dynamic array of elements for drawing quads and(or) triangles. Im vbo vertices(corners) of quad must be in order [bl -> tr -> tl -> br].

Params:
rElementsCount - set count of elements in ebo (ZC_DrawElements>(..., elementsCount, ..., ...);)
rElementsType - set elements type in ebo (ZC_DrawElements>(..., ..., elementsType, ...);)
quadsCount - count of quads in vbo.
trianglesCount - count of triangles in vbo.

Return:
Array of element in uchar format. Using:
    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ebo.BufferData(elements.size, elements.Begin(), ...);
*/
ZC_DA<uchar> GetTriangleElements(size_t& rElementsCount, GLenum& rElementsType, size_t quadsCount, size_t trianglesCount)
{
    size_t quadsElementsCount = quadsCount * 6,     //  6 elements in ebo on one quad
        trianglesElementsCount = trianglesCount * 3;     //  3 elements in ebo on one triangle
    rElementsCount = quadsElementsCount + trianglesElementsCount;  
    size_t verticesInVBO = (quadsCount * 4) + trianglesElementsCount,     //  4 vertices in vbo on one quad
        storingTypeSize = 0;
    ZC_Buffer::GetElementsData(verticesInVBO - 1, storingTypeSize, rElementsType);
    ZC_DA<uchar> elements(storingTypeSize * rElementsCount);
    switch (storingTypeSize)
    {
    case 1: FillTriangleElements(elements.pHead, elements.size, quadsElementsCount); break;
    case 2: FillTriangleElements(reinterpret_cast<ushort*>(elements.pHead), elements.size / 2, quadsElementsCount); break;
    case 4: FillTriangleElements(reinterpret_cast<uint*>(elements.pHead), elements.size / 4, quadsElementsCount); break;
    }
    return elements;
}

struct ZC_TextSharedData
{
    std::string text;
    ZC_TextAlignment alignment;
    ZC_RendererSet rendererSet;
};

struct ZC_TextData
{
public:
    virtual ~ZC_TextData() = default;

    void NeedDraw(bool needDraw)
    {
        needDraw ? rsController.SwitchToLvl(isParantOf_ZCTextWindow ? ZC_RendererLevels::TextScene : ZC_RendererLevels::TextWindow)
            : rsController.SwitchToLvl(ZC_RL_None);
    }

    void SetColor(const ZC_Vec3<float>& color)
    {
        uint colorUint = ZC_PackColorFloatToUInt(color[0], color[1], color[2]);
        ZC_RSPDUniformData unColor(ZC_UN_unColor, &colorUint);
        rsController.SetData(ZC_RSPDC_uniforms, &unColor);
    }

    void SetText(const std::string& _text)
    {
        if (spTextSharedData->text == _text) return;
        spTextSharedData->text = _text;

        //  change data in ZC_DrawElements don't changing pointer in spTextSharedData->rendererSet.upGLDraw
        dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->rendererSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
            CalculateAndSetTextData(*(++(spTextSharedData->rendererSet.buffers.begin())), *(spTextSharedData->rendererSet.buffers.begin()),
            spTextSharedData->text, spTextSharedData->alignment);
        SetNewTextSize();
    }

    void SetAlignment(ZC_TextAlignment _alignment)
    {
        if (spTextSharedData->alignment == _alignment) return;
        spTextSharedData->alignment = _alignment;

        //  change data in ZC_DrawElements don't changing pointer in spTextSharedData->rendererSet.upGLDraw
        dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->rendererSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
            CalculateAndSetTextData(*(++(spTextSharedData->rendererSet.buffers.begin())), *(spTextSharedData->rendererSet.buffers.begin()),
            spTextSharedData->text, spTextSharedData->alignment);
    }

    //  If need to change text and alignment at the same time, use this function. Calling SetText() and SetAlignment() separately is less effective.
    void SetTextAndAlignment(const std::string& _text, ZC_TextAlignment _alignment)
    {
        bool recalculateText = false;
        if (spTextSharedData->text != _text)
        {
            spTextSharedData->text = _text;
            recalculateText = true;
        }
        bool recalculateAlignment = false;
        if (spTextSharedData->alignment != _alignment)
        {
            spTextSharedData->alignment = _alignment;
            recalculateAlignment = true;
        }
        if (recalculateText || recalculateAlignment)
            dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->rendererSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
                CalculateAndSetTextData(*(++(spTextSharedData->rendererSet.buffers.begin())), *(spTextSharedData->rendererSet.buffers.begin()),
                spTextSharedData->text, spTextSharedData->alignment);

        if (recalculateText) SetNewTextSize();
    }

protected:
    typedef typename ZC_Fonts::Font Font;
    Font* pFont;
    bool isParantOf_ZCTextWindow;
    ZC_sptr<ZC_TextSharedData> spTextSharedData;
    ZC_RSController rsController;
    float textWidth,
        textHeight;

    ZC_TextData(bool _isParantOf_ZCTextWindow, typename ZC_Fonts::NameHeight fontData, const std::string& _text,
            ZC_TextAlignment _alignment)
        : pFont(ZC_Fonts::GetFont(fontData)),
        isParantOf_ZCTextWindow(_isParantOf_ZCTextWindow),
        spTextSharedData(ZC_sptrMake<ZC_TextSharedData>(_text, _alignment, MakeRendererSet(_text, _alignment))),
        rsController(spTextSharedData->rendererSet.MakeZC_RSController())
    {
        rsController.pTexture = &(pFont->texture);  //  the texture pointer is manually set because in the case of Text one texture can be used in multiple objects and is stored in Font::Font and not in ZC_RendererSet::texSets
        rsController.texturesCount = 1;

        NeedDraw(true);
    }

    ZC_TextData(const ZC_TextData& td)
        : pFont(td.pFont),
        isParantOf_ZCTextWindow(td.isParantOf_ZCTextWindow),
        spTextSharedData(td.spTextSharedData),
        rsController(td.rsController.MakeCopy()),
        textWidth(td.textWidth),
        textHeight(td.textHeight)
    {
        NeedDraw(true);
    }

private:
    virtual void SetNewTextSize() {};
    
    ZC_RendererSet MakeRendererSet(const std::string& _text, ZC_TextAlignment _alignment)
    {
        auto pShPIS = ZC_ShProgs::Get(isParantOf_ZCTextWindow ? ZC_ShProgs::Name::ZC_TextWindow : ZC_ShProgs::Name::ZC_TextScene);
        ZC_Buffer vbo(GL_ARRAY_BUFFER);
        ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
        ZC_VAO vao;
        vao.Config(pShPIS->vaoConfigData, vbo, &ebo, 0, 0);

        auto upGLDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(CalculateAndSetTextData(vbo, ebo, _text, _alignment));

        std::forward_list<ZC_Buffer> buffers;
        buffers.emplace_front(std::move(vbo));
        buffers.emplace_front(std::move(ebo));

        return ZC_RendererSet(pShPIS, std::move(vao), std::move(upGLDraw), std::move(buffers));
    }

    ZC_DrawElements CalculateAndSetTextData(ZC_Buffer& rVBO, ZC_Buffer& rEBO, const std::string& text, ZC_TextAlignment alignment)
    {
        textWidth = 0;
        textHeight = 0;
        auto pointsCoords = pFont->FillCoords(text, !isParantOf_ZCTextWindow, alignment, textWidth, textHeight);

        rVBO.BufferData(pointsCoords.size() * sizeof(typename Font::Point), &(pointsCoords[0]), GL_STATIC_DRAW);
  
        size_t elementsCount = 0;
        GLenum elementsType = 0;
        ZC_DA<uchar> elements = GetTriangleElements(elementsCount, elementsType, pointsCoords.size() / 4, 0);

        rEBO.BufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);
        //  creates on stack ZC_DrawElements cause in MakeRendererSet() will created in heap ZC_uptr<ZC GLDraw>, in other functions updates data created in MakeRendererSet()
        return { GL_TRIANGLES, static_cast<int>(elementsCount), elementsType, 0 };
    }
};

//  Class for rendering text into the window.
class ZC_TextWindow : public ZC_WindowOrthoIndent, public ZC_TextData
{
public:
    /*
    Params:
    fontData - fonts name and size.
    _text - text for rendering (supports'\n' new line symbol).
    _alignment - alignment across text consisting of several lines of different lengths.
    color - texts color.
    windowIndentX - value of horizontal indent from border of global window. If used IndentFlag:
        ZC_WOIF__X_Left_Pixel, ZC_WOIF__X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        ZC_WOIF__X_Left_Percent, ZC_WOIF__X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__X_Center -> value no metter.
    windowIndentY - value of vertival indent from border of global window. If used IndentFlag: 
        ZC_WOIF__Y_Top_Pixel, ZC_WOIF__Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        ZC_WOIF__Y_Top_Percent, ZC_WOIF__Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__Y_Center -> value no metter.
    indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    */
    ZC_TextWindow(typename ZC_Fonts::NameHeight fontData, const std::string& _text, ZC_TextAlignment _alignment,
            float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags)
        : ZC_WindowOrthoIndent(false, 0, 0, windowIndentX, windowIndentY, indentFlags),
        ZC_TextData(true, fontData, _text, _alignment)
    {
        SetNewTextSize();

        ZC_RSPDUniformData unPosition(ZC_UN_unPosition, this->currentIndents);
        rsController.SetData(ZC_RSPDC_uniforms, &unPosition);
    }

    /*
    Params:
    windowIndentX - value of horizontal indent from border of global window. If used IndentFlag:
        ZC_WOIF__X_Left_Pixel, ZC_WOIF__X_Right_Pixel -> value must be not negative, otherwise sets 0.f;
        ZC_WOIF__X_Left_Percent, ZC_WOIF__X_Right_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__X_Center -> value no metter.
    windowIndentY - value of vertival indent from border of global window. If used IndentFlag: 
        ZC_WOIF__Y_Top_Pixel, ZC_WOIF__Y_Bottom_Pixel -> value must be not negative, otherwise sets 0.f
        ZC_WOIF__Y_Top_Percent, ZC_WOIF__Y_Bottom_Percent -> value must be 0.0f - 1.f (where 1.f is 100%);
        ZC_WOIF__Y_Center -> value no metter.
    indentFlags - flags of indent horizontal(X) and vertical(Y) from border of global window to IGWindow. Must be set one flag for X and one flag for Y. Example: X_Left_Pixel | Y_Top_Pixel.
    */
    void SetIndentData(float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags)
    {
        this->SetNewIndentParams(windowIndentX, windowIndentY, indentFlags);
    }

    /*
    Makes a copy of the text.
    The methods affect only the current copy: NeedDraw(), SetColor(), SetIndentData().
    Methods that include effects on all copies: SetText() SetAlignment() SetTextAndAlignment().
    */
    ZC_TextWindow MakeCopy(float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags)
    {
        ZC_TextWindow copy = { *this };
        copy.SetIndentData(windowIndentX, windowIndentY, indentFlags);
        return copy;
    } 

private:
    ZC_TextWindow(const ZC_TextWindow& tw)
        : ZC_WindowOrthoIndent(dynamic_cast<const ZC_WindowOrthoIndent&>(tw)),
        ZC_TextData(dynamic_cast<const ZC_TextData&>(tw))
    {
        rsController.SetUniformsData(ZC_UN_unPosition, this->currentIndents);
    }

    void SetNewTextSize() override
    {
        this->SetNewSize(textWidth, textHeight);    //  calculates text indents into the window and store text size in ZC_WindowOrthoIndent
    }
};

//  Class for rendering text into the scene.
class ZC_TextScene : public ZC_TextData
{
public:
    /*
    Params:
    fontData - fonts name and size.
    _text - text for rendering (supports'\n' new line symbol).
    _alignment - alignment across text consisting of several lines of different lengths.
    color - texts color.
    */
    ZC_TextScene(typename ZC_Fonts::NameHeight fontData, const std::string& _text, ZC_TextAlignment _alignment)
        : ZC_TextData(false, fontData, _text, _alignment)
    {
        rsController.SetUniformsData(ZC_UN_unModel, &(ZC_Mat4<float>(1.f).Scale(scale, scale, scale)));
    }

    void SetPosition(const ZC_Vec3<float>& pos) noexcept
    {
        position = pos;
        RecalculateModelMatrix();
    }

    void SetRotation(float angle, const ZC_Vec3<float>& axises) noexcept
    {
        rotationAngle = angle;
        rotationAxises = axises;
        RecalculateModelMatrix();
    }

    void SetScale(float _scale)
    {
        scale = _scale;
        RecalculateModelMatrix();
    }

    //  This function must be used if more than one parameter of the model matrix(position, rotaion, scale) needs to be changed. 
    void SetModelMatrixData(ZC_Vec3<float>* pPosition, float* pRotationAngle, const ZC_Vec3<float>* pRotationAxises, float* pScale)
    {
        if (pPosition) position = * pPosition;
        if (pRotationAngle && pRotationAxises)
        {
            rotationAngle = *pRotationAngle;
            rotationAxises = *pRotationAxises;
        }
        if (pScale) scale = *pScale;
        RecalculateModelMatrix();
    }

    /*
    Makes a copy of the text.
    The methods affect only the current copy: NeedDraw(), SetColor(), SetPosition(), SetRotation(), SetScale(), SetModelMatrixData().
    Methods that include effects on all copies: SetText() SetAlignment() SetTextAndAlignment().
    */
    ZC_TextScene MakeCopy(ZC_Vec3<float>* pPosition, float* pRotationAngle, const ZC_Vec3<float>* pRotationAxises, float* pScale)
    {
        ZC_TextScene copy = { *this };
        copy.SetModelMatrixData(pPosition, pRotationAngle, pRotationAxises, pScale);
        return copy;
    } 

private:
    float scale = 0.01f;
    ZC_Vec3<float> position{ 0.f, 0.f, 0.f };
    float rotationAngle = 0.f;
    ZC_Vec3<float> rotationAxises{ 0.f, 0.f, 0.f };

    ZC_TextScene(const ZC_TextScene& tw)
        : ZC_TextData(dynamic_cast<const ZC_TextData&>(tw)),
        scale(tw.scale),
        position(tw.position),
        rotationAngle(tw.rotationAngle),
        rotationAxises(tw.rotationAxises)
    {
        rsController.SetUniformsData(ZC_UN_unModel, &(ZC_Mat4<float>(1.f).Scale(scale, scale, scale)));
    }

    void RecalculateModelMatrix()
    {
        ZC_Mat4<float> model(1.f);
        model.Translate(position);
        if (rotationAngle != 0.f) model.Rotate(rotationAngle, rotationAxises);
        model.Scale(scale, scale, scale);
        rsController.SetUniformsData(ZC_UN_unModel, &model);
    }
};

                                        //     add ZC_RLDText
// ZC_TextWindow* pText;
// void SetColor(float);
int ZC_main()
{
    using namespace ZC_Window;
    ZC_Window::MakeWindow(ZC_Window_Multisampling_4 | ZC_Window_Border, 800.f, 600.f, "ZeroCreator");
    // window->SetFPS(0);

    size_t textHeight = 200;
    ZC_Fonts::NameHeight fonts{ZC_FontName::Arial, textHeight};
    ZC_Fonts::Load(&fonts, 1);

    ZC_Window::GlClearColor(0.3f, 0.3f, 0.3f, 1.f);
    ZC_Window::GlEnablePointSize();
    
    ZCR_Scene scene;
    
    static const ulong firstASCII = 32,
        lastASCII = 127;
    char str[(lastASCII - firstASCII) * 2];
    for (size_t i = 0, strIndex = 0; i < (lastASCII - firstASCII) * 2; i++, strIndex++)
    {
        str[i] = firstASCII + strIndex;
        ++i;
        if (lastASCII - firstASCII == i)
            str[i] = '\n';
        else
            str[i] = ' ';
    }
    

    ZC_TextScene text1({ZC_FontName::Arial, textHeight}, str, ZC_TextAlignment::Left);
    text1.SetColor({1.f, 0.f, 0.f});
    ZC_Vec3<float> text2Pos(0.f, 5.f, 0.f);
    float text2RotAngle = 30.f;
    ZC_Vec3<float> text2Rotaxises(0.f, 0.f, 1.f);
    
    auto text2 = text1.MakeCopy(&text2Pos, &text2RotAngle, &text2Rotaxises, nullptr);
    text2.SetColor({0.f, 0.f, 1.f});

    ZC_TextWindow text({ZC_FontName::Arial, textHeight}, str, ZC_TextAlignment::Left, 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Top_Pixel);


    // pText = &text;
    // auto text1 = text.MakeCopy();
    // text1.SetColor({0, 1.f, 0});
    // text1.SetIndentData(20.f, 40.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel);

    // ZC_Events::ConnectButtonDown(ZC_ButtonID::K_Q, {&SetColor});

    ZC_Window::RuntMainCycle();
    
    return 0;
}

std::string first = "first\nLol\nsdaasdffffffafsdf\nsdaf",
    second = "{seconD}}\nasdf'_",
    current = first;


// void SetColor(float)
// {
//     if (current == first)
//     {
//         // pText->NeedDraw(false);
//         // pText->SetTextAndAlignment(second, ZC_TextAlignment::Left);
//         // pText->SetColor({1.f, 1.f, 0.f});
//         pText->SetIndentData(0.2f, 90.f, ZC_WOIF__X_Left_Percent | ZC_WOIF__Y_Center);
//         current = second;
//     }
//     else
//     {
//         // pText->NeedDraw(true);
//         // pText->SetTextAndAlignment(first, ZC_TextAlignment::Right);
//         // pText->SetColor({0.f, 1.f, 1.f});
//         pText->SetIndentData(20.f, 40.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel);
//         current = first;
//     }
// }
        

//     enum En
// {
//     null    = 0,
//     one     = 1 << 1,
//     two     = 1 << 2,
//     three   = 1 << 3,
//     four    = 1 << 4,
//     five    = 1 << 5,
//     six     = 1 << 6,
//     seven   = 1 << 7,
//     eight   = two | four,
// };
//     auto flags = En::one | En::eight | En::five;
//     if ((flags & En::three) == En::three)
//     {
//         int a = 3;
//     }

// class A
// {
// public:
//    A() = default;
//    explicit A(int _a) : a(_a){
//     ZC_cout("A ctr " + std::to_string(a));
//     }
//    A(const A& _a)
//     // = delete;
//     : a(_a.a)
//    {
//     ZC_cout("A() copy");
//     }
//    A& operator = (const A& _a)
//     // = delete;
//    {a = _a.a; 
//    ZC_cout("A = copy");
//     return *this;}
//    A(A&& _a)  noexcept : a(_a.a){_a.a = 0;
//    ZC_cout("A() move");
//    }
//    A& operator = (A&& _a) noexcept {a = _a.a; _a.a = 0;
//     ZC_cout("A = move");
//      return *this;}
//    virtual ~A(){
//     ZC_cout("A dtr " + std::to_string(a)); b = false;
//     }

//     bool operator < (const A& _a) const {return a < _a.a;}
//   //  virtual void Foo(){ZC_cout("A");}
//    int a = 0;
//    bool b = true;
// protected:
// private:
// };
// class B : public A
// {
// public:
//    B() = delete;
//    explicit B(int _a) : A(_a) {ZC_cout("B ctr");}
//    B(const B& _a) : A(dynamic_cast<const A&>(_a)){ZC_cout("B() copy");}
//    B& operator = (const B& _a){dynamic_cast<A&>(*this) = dynamic_cast<const A&>(_a); ZC_cout("B = copy"); return *this;}
//    B(B&& _a)  noexcept : A(dynamic_cast<A&&>(_a)){ZC_cout("B() move");}
//    B& operator = (B&& _a) noexcept {dynamic_cast<A&>(*this) = dynamic_cast<A&&>(_a); ZC_cout("B = move"); return *this;}
//    ~B() override{ZC_cout("B dtr");}
// };


        // //  Calculate a point on a 2D ellipse in the direction from the center of the ellipse to the corner.
        // ZC_Vec3<float> EllipsePoint(const ZC_Vec3<float>& ellipsesCenter, const ZC_Vec3<float>& ellipsX, const ZC_Vec3<float>& ellipsY, const ZC_Vec3<float>& corner)
        // {
        //     ZC_Vec3<float> direction = corner - ellipsesCenter;    //  direction of the desired point
        //     float radiusX = ZC_Vec::Length(ellipsX - ellipsesCenter),
        //         radiusY = ZC_Vec::Length(ellipsY - ellipsesCenter);
            
        //     //  if it's square, move the elipsCenter in the direction of the center by the length of radiusX or radiusY
        //     if (radiusX == radiusY) return ZC_Vec::MoveByLength(ellipsesCenter, direction, radiusX);

        //     //  ellips path https://ip76.ru/theory-and-practice/ellipse-point-coord/#eq_main
        //     //  work with triangle (ellipsCenter, center, ellipsX).
        //     float a = ZC_Vec::Length(corner - ellipsX),
        //         b = ZC_Vec::Length(corner - ellipsesCenter),  //  c - is radiusX
        //         cosA = ((b * b) + (radiusX * radiusX) - (a * a)) / (2.f * b * radiusX),   //  cosine theorema
        //         sinA = sqrt(1.f - (cosA * cosA)),   //  main trigonometric identity
        //         ellipsesAngle = atan2(radiusX * sinA, radiusY * cosA),  //  follow the link earlier, angleB
        //         x = radiusX * cos(ellipsesAngle),   //  x,y position of (center) on ellips
        //         y = radiusY * sin(ellipsesAngle),
        //         lengthXY = sqrt((x * x) + (y * y));
        //     return ZC_Vec::MoveByLength(ellipsesCenter, direction, lengthXY);
        // }
        
    //  упаковать цвет из float в uint
    // auto lambda = [](float r, float g, float a)
    // {
    //     return ((unsigned int)(r * 255.f) << 8 | (unsigned int)(g * 255.f)) << 8 | (unsigned int)(a * 255.f);
    // };
    //  распаковать из uint во float
    // unsigned int q = (r << 8 | g) << 8 | b;
    // float b = white << 24 >> 24,
    //     g = white << 16 >> 24,
    //     r = white >> 16;

//  упаковка r g b из диапазона (0, 255) в unsigned int
// #define Func(r, g, b){ ((unsigned int)r << 8 | g) << 8 | b }


    // //  texture loading
    // auto vectorOfTexturesCreator = pShPIS->texSets.GetCreator();
    // typedef typename ZC_TexSets::TextureName TexName;
    // for (auto pTextureName = vectorOfTexturesCreator.NextName(); pTextureName != nullptr; pTextureName = vectorOfTexturesCreator.NextName())
    // {
    //     switch (*pTextureName)
    //     {
    //     case TexName::texColor:
    //         vectorOfTexturesCreator.Add(ZC_Textures::LoadTexture2D(ZC_FSPath(ZCR_ZCRTexturePath).append("mesh.png").c_str())); break;
    //     }
    // }
    // std::vector<typename ZC_RSTexs::TexSet> texSets;
    // texSets.emplace_back(texSetName, vectorOfTexturesCreator.GetVector());

    // return { new ZC_RSTexs(pShPIS, std::move(vao), std::move(upDraw), std::move(buffers), std::move(texSets)) };




    // ZC_Sounds::LoadWAV("lp", "/home/dmitry/Загрузки/lp.wav");
    // ZC_upSound lp = ZC_Sounds::GetSound("lp");
    // ZC_Sounds::LoadWAV("Airplanes", "/home/dmitry/Загрузки/Airplanes.wav");
    // ZC_upSound airplanes = ZC_Sounds::GetSound("Airplanes");
    // ZC_Audio::OpenAudioStream(ZC_AudioSet(ZC_AudioSet::Channels::Stereo, 44100, ZC_AudioSet::BitsPerSample::S16));
    // lp->Play();
    // airplanes->Play();