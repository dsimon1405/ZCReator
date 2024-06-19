#include <ZC/main/ZC_main.h>
#include <ZC/Video/ZC_SWindow.h>
#include <Objects/Scene/ZCR_Scene.h>
#include <ZCR_Load.h>
#include <ZC/Tools/Time/ZC_Clock.h>

#include <glad/glad.h>

#include <iostream>


//  ZC_ZoneSelector, algorithm -> Connect click left mouse button (now can't be connects one button twisem=, take care). If button down heppens, check ereas: ZC_MouseCollisionWindowController::IsCursorInArea(), and ImGui.
//  If that ereas don't use cursor, connect mouse move event and disable ZC_MouseCollisionWindowController events (don't created yet), try to find how to disable ImGui events.
//  - If heppens move create ortho quad textured or colored with alpha. On left mouse button up selected zone give those who need that info, and make next step. 
//  - If heppens left mouse button up enable ZC_MouseCollisionWindowController and ImGui events if disconnected.


#include <Renderer/ZCR_ShaderKeeper.h>

// void Create()
// {
    // GLuint tex;
    // glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    // glTextureStorage2D(tex, 1, GL_RG8, 2, 2);
    // ZC_Vec2<GLubyte> colors[]
    // {
    //     {1,2}, {3,4},
    //     {5,6}, {7,8}
    // };
    // GLubyte colors1[]
    // {
    //     1,2,3,4
    // };

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // glTextureSubImage2D(tex, 0, 0, 0, 2, 2, GL_GREEN, GL_UNSIGNED_BYTE, colors1);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // GLubyte get[sizeof(colors)];
    // std::fill(get, get + sizeof(get), 0);
    // glGetTextureImage(tex, 0, GL_RG, GL_UNSIGNED_BYTE, sizeof(colors), get);
    
    // int a = 3;
// }
#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/GUI/ZC_GUI_WinMutable.h>
#include <ZC/GUI/ZC_GUI_Button.h>
#include <ZC/Events/ZC_Events.h>

// ZC_uptr<ZC_GUI> pGUI;   //  must be in ZC_SWindowHolder
struct ZC_W
{
    ZC_uptr<ZC_GUI_Window> pWin;
    ZC_uptr<ZC_GUI_Button> pBtn1;
    ZC_uptr<ZC_GUI_Button> pBtn2;
    ZC_uptr<ZC_GUI_Button> pBtn3;
    ZC_uptr<ZC_GUI_Button> pBtn4;
    ZC_W() = default;
    ZC_W(float indentX, float indexntY)
    {
        pWin = new ZC_GUI_WinMutable(ZC_WOIData{.width = 200, .height = 200, .indentX = indentX, .indentY = indexntY, .indentFlags = ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel},
            ZC_UV{.bl{0,0}, .tr{1,1}},
             ZC_GUI_WF__Movable
            );

        pBtn1 = new ZC_GUI_Button(30, 30);
        pBtn2 = new ZC_GUI_Button(30, 30);
        pWin->AddRow({.indent_x = 10, .indent_y = 0, .distance_x = 30, .height = 20});
        pWin->AddObj(pBtn1.Get());
        pWin->AddObj(pBtn2.Get());

        pBtn3 = new ZC_GUI_Button(10, 20);
        pBtn4 = new ZC_GUI_Button(20, 200);
        pWin->AddRow({.indent_x = 5, .indent_y = 100, .distance_x = 10, .height = 0});
        pWin->AddObj(pBtn3.Get());
        pWin->AddObj(pBtn4.Get());
    }    
};

ZC_W* win1;
ZC_W* win2;
ZC_W* win3;

void Draw1(ZC_ButtonID,float) { win1->pWin->VSetDrawState_W(true); }
void NoDraw1(ZC_ButtonID,float) { win1->pWin->VSetDrawState_W(false); }
void Focuse1(ZC_ButtonID,float) { win1->pWin->MakeForcused(); }
void Draw2(ZC_ButtonID,float) { win2->pWin->VSetDrawState_W(true); }
void NoDraw2(ZC_ButtonID,float) { win2->pWin->VSetDrawState_W(false); }
void Focuse2(ZC_ButtonID,float) { win2->pWin->MakeForcused(); }
void Draw3(ZC_ButtonID,float) { win3->pWin->VSetDrawState_W(true); }
void NoDraw3(ZC_ButtonID,float) { win3->pWin->VSetDrawState_W(false); }
void Focuse3(ZC_ButtonID,float) { win3->pWin->MakeForcused(); }

int ZC_main()
{
    using namespace ZC_SWindow;
    ZC_SWindow::MakeWindow(
        ZC_Window_Multisampling_4 | 
        ZC_Window_Border, 800, 600, "ZeroCreator");
    ZC_SWindow::SetFPS(0);
    ZC_SWindow::NeedDrawFPS(true);

    // ZC_GUI::Init();

    ZCR_ShaderLoader::LoadAllShaders();

    // ZC_ShaderPrograms::CompilationTest(
    //     "/home/dmitry/projects/ZCreator/build/assets/ZC/shaders/GUI/gui.vs",
    //     nullptr,
    //     nullptr,
    //     "/home/dmitry/projects/ZCreator/build/assets/ZC/shaders/GUI/gui.gs",
    //     "/home/dmitry/projects/ZCreator/build/assets/ZC/shaders/GUI/gui.fs");

    ZCR_LoadFonts();
    
    ZCR_Scene scene;


    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_Q, {}, Draw1);
    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_W, {}, NoDraw1);
    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_E, {}, Focuse1);
    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_A, {}, Draw2);
    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_S, {}, NoDraw2);
    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_D, {}, Focuse2);
    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_Z, {}, Draw3);
    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_X, {}, NoDraw3);
    ZC_Events::ConnectButtonClick(ZC_ButtonID::K_C, {}, Focuse3);

    win1 = new ZC_W(10, 10);
    win2 = new ZC_W(50, 50);
    win3 = new ZC_W(100, 100);

    // win2.pWin->VSetDrawState(true);
    // win1.pWin->VSetDrawState(true);
    // win3.pWin->VSetDrawState(true);


    ZC_SWindow::RuntMainCycle();
    
    delete win1;
    delete win2;
    delete win3;
    return 0;
}


        // #include <ZC/Tools/Time/ZC_Clock.h>
        // #include <iostream>
        // ZC_Clock cl;
        // size_t count = 1000000;

        // cl.Start();
        // for (size_t i = 0; i < count; i++) 
        // std::cout<<cl.Time<ZC_Nanoseconds>()<<std::endl;

// #include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
// #include <ZC/Video/OpenGL/VAO/ZC_VAOs.h>
// #include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>
// #include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
// #include <ZC/Video/OpenGL/Buffer/ZC_Buffers.h>
// #include <ZC/Objects/Camera/ZC_Camera.h>

// #include <ZC/Tools/Time/ZC_Clock.h>

// #include <ZC/Tools/Math/ZC_Mat.h>
// #include <ZC/Tools/Console/ZC_cout.h>

// #include <ZC/Events/ZC_Events.h>

// #include <ZC/Objects/ZC_Object.h>

// #include <ZC/Tools/Math/ZC_Figures.h>

// #include <ZC/Video/OpenGL/Texture/ZC_Texture.h>

// // #include <Objects/ImGui/ZCR_ImGui.h>
// #include <ZC/Tools/Container/ZC_ContFunc.h>


// #include <ZC/Objects/Text/ZC_Fonts.h>
// #include <ZC/Objects/Text/ZC_TextWindow.h>
// #include <ZC/Objects/Text/ZC_TextScene.h>
// #include <ZC/Objects/Text/ZC_TextSceneTurnedToCamera.h>
// #include <ZC/Objects/Text/ZC_TextWindowIntoScene.h>

// #include <Renderer/ZCR_RendererLevelDrawer.h>
// #include <ZC/Video/OpenGL/ZC_Framebuffer.h>
// #include <ZC/Video/OpenGL/ZC_Viewport.h>

// #include <ZC/Video/OpenGL/ZC_FBO.h>
// #include <ZC/Tools/ZC_OrthoSquare.h>
// #include <ZC/Collision/ZC_MouseCollisionWindow.h>

// #include <ZC/Events/ZC_ESignal.h>


// #include <ZC/Tools/ZC_ButtonManipulator.h>


// struct NewSquare
// {
//     ZC_Quad bl;
//     ZC_Quad tl;
//     ZC_Quad tr;
//     ZC_Quad br;
//     void SetCenter(const ZC_Vec3<float>& center) { bl.tr = tl.br = tr.bl = br.tl = center; }
//     ZC_Vec3<float>& GetCenter() { return bl.tr; }
//     void SetEdgePointL(const ZC_Vec3<float>& point) { bl.tl = tl.bl = point; }
//     void SetEdgePointT(const ZC_Vec3<float>& point) { tl.tr = tr.tl = point; }
//     void SetEdgePointR(const ZC_Vec3<float>& point) { tr.br = br.tr = point; }
//     void SetEdgePointB(const ZC_Vec3<float>& point) { bl.br = br.bl = point; }
// };

// struct Square
// {
//     ZC_Vec3<float> edgeCenterL,
//         edgeCenterT,
//         edgeCenterR,
//         edgeCenterB;
//     bool isEdgePointL = false,  //  is left edge point calculated
//         isEdgePointT = false,
//         isEdgePointR = false,
//         isEdgePointB = false;
// };

// struct VertexData
// {
//     NewSquare& rNS;
//     ZC_Vec3<float>& rNewPoint,
//         & rEC1,
//         & rEC2,
//         & rCorner1,
//         & rCorner2;
//     void(NewSquare::*pSetEP1)(const ZC_Vec3<float>&);
//     void(NewSquare::*pSetEP2)(const ZC_Vec3<float>&);
//     bool& rIsEP1,
//         rIsEP2;

//     VertexData(NewSquare& _rNewSquare,
//                 ZC_Vec3<float>& _rNewPoint,
//                 ZC_Vec3<float>& _rEdgeCenter1,
//                 ZC_Vec3<float>& _rEdgeCenter2,
//                 ZC_Vec3<float>& _rCorner1,
//                 ZC_Vec3<float>& _rCorner2,
//                 void(NewSquare::*_pSetEdgePoint1)(const ZC_Vec3<float>&),
//                 void(NewSquare::*_pSetEdgePoint2)(const ZC_Vec3<float>&),
//                 bool& _rIsEdgePoint1,
//                 bool& _rIsEdgePoint2)
//     : rNS(_rNewSquare),
//     rNewPoint(_rNewPoint),
//     rEC1(_rEdgeCenter1),
//     rEC2(_rEdgeCenter2),
//     rCorner1(_rCorner1),
//     rCorner2(_rCorner2),
//     pSetEP1(_pSetEdgePoint1),
//     pSetEP2(_pSetEdgePoint2),
//     rIsEP1(_rIsEdgePoint1),
//     rIsEP2(_rIsEdgePoint2)
//     {}
// };

// struct CC
// {
//     struct Vertex
//     {
//         ZC_Vec3<float> vertex;
//         std::list<VertexData> vdList;

//         bool operator == (const ZC_Vec3<float>& _v) const noexcept
//         {
//             return vertex == _v;
//         }
//     };

//     ZC_DA<Square> squares;
//     NewSquare* newSquares;
//     ulong squaresCount;
//     std::list<Vertex> vertices;

//     CC(ZC_DA<ZC_Quad>& quads)
//         : squares(quads.size),
//         newSquares(new NewSquare[quads.size])
//     {
//         for (ulong _squaresIndex = 0; _squaresIndex < quads.size; ++_squaresIndex)
//         {
//             Square& rSSq = squares[_squaresIndex];
//             ZC_Quad* pQSq = quads.pHead;
//             rSSq.edgeCenterL = (pQSq->bl + pQSq->tl) / 2.f;
//             rSSq.edgeCenterT = (pQSq->tl + pQSq->tr) / 2.f;
//             rSSq.edgeCenterR = (pQSq->tr + pQSq->br) / 2.f;
//             rSSq.edgeCenterB = (pQSq->br + pQSq->bl) / 2.f;
//             NewSquare& rNS = newSquares[_squaresIndex];
//             rNS.SetCenter(quads.pHead->Center());
//             Add(pQSq->bl, VertexData(rNS, rNS.bl.bl, rSSq.edgeCenterB, rSSq.edgeCenterL, pQSq->br, pQSq->tl,
//                 &NewSquare::SetEdgePointB, &NewSquare::SetEdgePointL, rSSq.isEdgePointB, rSSq.isEdgePointL));
//             Add(pQSq->tl, VertexData(rNS, rNS.tl.tl, rSSq.edgeCenterT, rSSq.edgeCenterL, pQSq->tr, pQSq->bl,
//                 &NewSquare::SetEdgePointT, &NewSquare::SetEdgePointL, rSSq.isEdgePointT, rSSq.isEdgePointL));
//             Add(pQSq->tr, VertexData(rNS, rNS.tr.tr, rSSq.edgeCenterT, rSSq.edgeCenterR, pQSq->tl, pQSq->br,
//                 &NewSquare::SetEdgePointT, &NewSquare::SetEdgePointR, rSSq.isEdgePointT, rSSq.isEdgePointR));
//             Add(pQSq->br, VertexData(rNS, rNS.br.br, rSSq.edgeCenterB, rSSq.edgeCenterR, pQSq->bl, pQSq->tr,
//                 &NewSquare::SetEdgePointB, &NewSquare::SetEdgePointR, rSSq.isEdgePointB, rSSq.isEdgePointR));
//         }
//     }
    
//     void Add(const ZC_Vec3<float>& vertex, const VertexData& vData)
//     {
//         auto pVertex = ZC_Find(vertices, vertex);
//         if (pVertex) pVertex->vdList.emplace_back(vData);
//         else vertices.emplace_back(Vertex{vertex, {vData}});
//     }

//     //  Catmull Clark https://en.wikipedia.org/wiki/Catmull%E2%80%93Clark_subdivision_surface
//     ZC_DA<ZC_Quad> Calculate()
//     {
//         for (auto& vert : vertices)
//         {
//             ulong faces = vert.vdList.size();
//             auto vdI = vert.vdList.begin();
//             if (faces == 1)   //  point alone
//             {
//                 //  set edges points as edges center
//                 (vdI->rNS.*vdI->pSetEP1)(vdI->rEC1);
//                 (vdI->rNS.*vdI->pSetEP2)(vdI->rEC2);
//                 //  move the center in the direction of the corner + edge centers, by an average length to the center of each edge
//                 //  (If the figure is a square, the result will be movement by the radius of the inscribed circle)
//                 vdI->rNewPoint = ZC_Vec::MoveByLength(vdI->rNS.GetCenter(), (vert.vertex + vdI->rEC1 + vdI->rEC2) / 3.f - vdI->rNS.GetCenter(),
//                     (ZC_Vec::Length(vdI->rNS.GetCenter() - vdI->rEC1) + ZC_Vec::Length(vdI->rNS.GetCenter() - vdI->rEC2)) / 2.f);
//                 continue;
//             }
    
//             ZC_Vec3<float> centersSum,
//                 commonEdgesCentersSum,
//                 singleEgdesCentersSum;
//             unsigned short commonEdges = 0;
//             unsigned short singleEdges = 0;
//             //  find edges points
//             for (; vdI != vert.vdList.end(); ++vdI)
//             {
//                 centersSum += vdI->rNS.GetCenter();
//                 for (auto vdI2 = vdI; vdI2 != vert.vdList.end() && (!vdI->rIsEP1 || !vdI->rIsEP2); ++vdI2)
//                 {
//                     if (!vdI->rIsEP1 && !vdI2->rIsEP1 && vdI->rEC1 == vdI2->rEC1)   //  common edge, calculate edge point
//                         CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC1, (vert.vertex + vdI->rCorner1 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
//                             vdI->rIsEP1, vdI2->rIsEP1, vdI->rNS, vdI2->rNS, vdI->pSetEP1, vdI2->pSetEP1, commonEdges);
//                     if (!vdI->rIsEP1 && !vdI2->rIsEP2 && vdI->rEC1 == vdI2->rEC2)
//                         CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC1, (vert.vertex + vdI->rCorner1 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
//                             vdI->rIsEP1, vdI2->rIsEP2, vdI->rNS, vdI2->rNS, vdI->pSetEP1, vdI2->pSetEP2, commonEdges);
//                     if (!vdI->rIsEP2 && !vdI2->rIsEP1 && vdI->rEC2 == vdI2->rEC1)
//                         CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC2, (vert.vertex + vdI->rCorner2 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
//                             vdI->rIsEP2, vdI2->rIsEP1, vdI->rNS, vdI2->rNS, vdI->pSetEP2, vdI2->pSetEP1, commonEdges);
//                     if (!vdI->rIsEP2 && !vdI2->rIsEP2 && vdI->rEC2 == vdI2->rEC2)
//                         CommonEdgeCenter(commonEdgesCentersSum, vdI->rEC2, (vert.vertex + vdI->rCorner2 + vdI->rNS.GetCenter() + vdI2->rNS.GetCenter()) / 4.f,
//                             vdI->rIsEP2, vdI2->rIsEP2, vdI->rNS, vdI2->rNS, vdI->pSetEP2, vdI2->pSetEP2, commonEdges);
//                 }  //  no common edges, set edge point as edge center
//                 if (!vdI->rIsEP1) SingleEdgeCenter(vdI->rEC1, vdI->rIsEP1, vdI->rNS, vdI->pSetEP1, singleEgdesCentersSum, singleEdges);
//                 if (!vdI->rIsEP2) SingleEdgeCenter(vdI->rEC2, vdI->rIsEP2, vdI->rNS, vdI->pSetEP2, singleEgdesCentersSum, singleEdges);
//             }

//             ZC_Vec3<float> newPoint;
//             if (singleEdges == 0) newPoint = ((centersSum / faces) + (commonEdgesCentersSum / commonEdges) * 2.f + vert.vertex * (commonEdges - 3.f)) / commonEdges;  //  barycenter
//             else if (singleEdges == 2) newPoint = (singleEgdesCentersSum + vert.vertex * 2.f) / 4.f;
//             else newPoint = vert.vertex;

//             for (auto& rvd : vert.vdList) rvd.rNewPoint = newPoint;
//         }

//         ZC_DA<ZC_Quad> result(reinterpret_cast<ZC_Quad*>(newSquares), squares.size * 4);
//         squares.pHead = nullptr;
//         return result;
//     }

//     void SingleEdgeCenter(const ZC_Vec3<float>& point, bool& rIsEP, NewSquare& rNS, void(NewSquare::*pSet)(const ZC_Vec3<float>&),
//         ZC_Vec3<float>& rSECS, unsigned short& rSingleEdges)
//     {
//         rIsEP = true;
//         (rNS.*pSet)(point);
//         rSECS += point;
//         ++rSingleEdges;
//     }

//     void CommonEdgeCenter(ZC_Vec3<float>& rCECSum, ZC_Vec3<float>& rCEC, const ZC_Vec3<float>& point, bool& rIsEP1, bool& rIsEP2, NewSquare& rNS1, NewSquare& rNS2,
//         void(NewSquare::*pSet1)(const ZC_Vec3<float>&), void(NewSquare::*pSet2)(const ZC_Vec3<float>&), unsigned short& rCommonEdges)
//     {
//         rCECSum += rCEC;
//         rIsEP1 = true;
//         (rNS1.*pSet1)(point);
//         rIsEP1 = true;
//         (rNS2.*pSet2)(point);
//         ++rCommonEdges;
//     }
// };





    /// TEXT

    // ulong textHeight = 150;
    // ulong textHeight1 = 151;
    // ZC_FontData fonts[]{ { ZC_FontName::ZC_F_Arial, textHeight },{ ZC_FontName::ZC_F_Arial, textHeight1 } };
    // ZC_Fonts::Load(fonts, 2);

    // std::string str = "Dimp\nL";

    // ZC_TextWindow text({ZC_F_Arial, textHeight}, str, ZC_TA_Left, 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel, true);
    // text.SetColorFloat(1,0,0);
    // text.SetAlpha(0.5f);

    // ZC_TextScene text1({ZC_F_Arial, textHeight}, str, ZC_TextAlignment::ZC_TA_Right, true);
    // text1.SetPosition({ 0,-4,0 });
    // text1.SetColorFloat(0,1,0);
    // // text1.SetRendererLevel(ZC_DrawerLevels::TextScene);
    // // text2.SetRendererLevel(ZC_DrawerLevels::TextScene);
    // ZC_TextSceneTurnedToCamera textSceneTurn({ZC_F_Arial, textHeight}, str, ZC_TextAlignment::ZC_TA_Right, {10.f, 3.f, 0.f}, true);
    // textSceneTurn.SetColorFloat(1,1,1);

    // ZC_TextWindowIntoScene textWIS({ZC_F_Arial, textHeight}, ZC_FO_bottomCenter, str, ZC_TextAlignment::ZC_TA_Center, {0.f, 0.f, 0.f}, true);
    // ZC_TextWindowIntoScene textWIS2({ZC_F_Arial, textHeight}, ZC_FO_bottomCenter, str, ZC_TextAlignment::ZC_TA_Center, {0.f, -5.f, 0.f}, true);
    // textWIS2.SetColorFloat(0,0,1.f);
    // // ZC_TextWindowIntoScene text2({ZC_F_Arial, textHeight1}, ZC_FO_center, str, ZC_TextAlignment::ZC_TA_Right, { 5, 3, 3 });
    // // text2.SetColorFloat(0,0,1);



//  pos = perspView * pos       -> with w divisor component
//  x = pos.x / pos.w; y = pos.y / pos.w        -> x, y [-1,1]
//  z = pos.z / pos.w       -> depth valid values [0,1] -> 0 closer, 1 further; if (> 1 and < 0) -> out of frustrum

//  pos = ortho * vec4      -> x, y [-1,1]
        


// void MouseMove(float x, float y, float xrel, float yrel, float time)
// {
//     ZC_cout("x = " + std::to_string(x) + "; [-1,1] = " + std::to_string(ZC_ToMinusPlusOneRange(x / 800.f))
//         + ";     y = " + std::to_string(y) + "; [-1,1] = " + std::to_string((1.f - (y / 600.f)) * 2 - 1.f));
// }


//  transparency => create framebuffer(if main multisampled create multisampled and then blit in another, created with texture2d attachment)
//  Color attachment finaly must be on one texture2d for each transparency object(don't forgot alpha channel in GL_RGBA or in separate uniform for each object).
//  Depth attachment uses main renderbuffer (0), but don't write there (glDepthMasr(GL_FALSE)),
//  cause if creates several transparency object they must have different fbo but use main depth rbo for depth testing with not transparent scene.
//  Drawing must be started after scene objects but before window objects(to avoid clearing of the depth buffer, like in ZC_TextWindowIntoScene::VDraw()).
//  Finaly create shader that may attach (teoreticly) 16 color textures (max count) (if alpha in separated uniforms, don't forgot them), there we blending the color of
//  each texture (transparency objects) with scene framebuffer (main buffer attachmant 0).

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