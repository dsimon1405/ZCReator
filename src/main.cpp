#include <ZC/main/ZC_main.h>
#include <ZC/Video/ZC_Video.h>

#include <ZC/Video/OpenGL/Shader/ZC_ShProgs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAOs.h>
#include <ZC/Video/OpenGL/VAO/ZC_VAOConfigs.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <ZC/Video/OpenGL/Buffer/ZC_Buffers.h>
#include <ZC/Objects/Camera/ZC_Camera.h>

#include <ZC/Tools/Time/ZC_Clock.h>

#include <ZC/Tools/Math/ZC_Mat.h>
#include <ZC/Tools/Console/ZC_cout.h>

#include <ZC/Events/Button/ZC_ButtonOperator.h>
#include <ZC/Events/ZC_Mouse.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include <ZC/Objects/ZC_Object.h>

#include <ZC/Video/OpenGL/Figure/ZC_Quad.h>

#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

#include <ZC/Video/OpenGL/Texture/ZC_Textures.h>
#include <Ogl.h>
#include <Objects/ZCR_Cube.h>


    float width = 800.f;
    float height = 600.f;
    ZC_Vec4<float> p1{0.f, -4.8f, 0.f, 1.f},
        p2(0.f, -4.78f, 0.f, 1.f),
        p3(1.f,-0.8f,1.f, 1.f);

    ZC_Vec3<float> camPos{0.f, -5.f, 0.f};
// x = 799.000000 = 0.997500; y = 0.000000 = 1.000000     {1.810660 2.414213 3.807808 4.000000}     {0.452665 0.603553 0.951952}     {0.452654 0.603538 0.951953}
struct Player
{
    ZC_Vec3<float> position;    //  не ведись, это look on
    float speed;
    ZC_Camera camera;
    ZC_Vec3<float> front {0.f, 1.f, 0.f};
    ZC_Vec3<float> up {0.f, 0.f, 1.f};
    ZC_Vec3<float> right {1.f, 0.f, 0.f};

    float camDistance = 5.f;
    float verticalAngle = 0.f;
    float horizontalAngle = 0.f;
    float sensitivity = 1.f;

    Player(const ZC_Vec3<float>& _position, float _speed)
        : position(_position),
        speed(_speed),
        camera(ZC_Camera({ZC_Perspective(45.f, 0.1f, 100.f), ZC_View({position[0], position[1] - 5.f, position[2]}, position, {0.f, 0.f, 1.f}, true)}, ZC_Ortho()))
    {
       ZC_ButtonOperator::Connect({ZC_ButtonID::K_W, ZC_Button::State::Down}, {&Player::W,this});
       ZC_ButtonOperator::Connect({ZC_ButtonID::K_S, ZC_Button::State::Down}, {&Player::S,this});
       ZC_ButtonOperator::Connect({ZC_ButtonID::K_A, ZC_Button::State::Down}, {&Player::A,this});
       ZC_ButtonOperator::Connect({ZC_ButtonID::K_D, ZC_Button::State::Down}, {&Player::D,this});
       ZC_ButtonOperator::Connect({ZC_ButtonID::K_Q, ZC_Button::State::Down}, {&Player::Q,this});
       ZC_ButtonOperator::Connect({ZC_ButtonID::K_E, ZC_Button::State::Down}, {&Player::E,this});
       ZC_Mouse::ConnectMove({&Player::Mouse, this});
    }

    void Move(const ZC_Vec3<float>& step) noexcept
    {
        position += step;
        camera.SetLookOn(position)
              .SetCamPos(camera.GetCamPos() + step);
    }
    
    void W(float time) {Move(front * time * speed);}
    void S(float time) {Move(front * -1 * time * speed);}
    void A(float time) {Move(right * -1 * time * speed);}
    void D(float time) {Move(right * time * speed);}
    void Q(float time) {Move(up * time * speed);}
    void E(float time) {Move(up * -1 * time * speed);}
    void Mouse(float x, float y, float xRel, float yRel, float time)
    {
        // ZC_cout("x = " + std::to_string(x) + " = " + std::to_string((x / width) * 2 - 1.f) + "; "
        //     + "y = " + std::to_string(y) + " = " + std::to_string((1.f - (y / height)) * 2 - 1.f)
        //     + "     {" + std::to_string(p1[0]) + " " + std::to_string(p1[1]) + " " + std::to_string(p1[2]) + " " + std::to_string(p1[3]) + "}"
        //     + "     {" + std::to_string(p1[0] / p1[3]) + " " + std::to_string(p1[1] / p1[3]) + " " + std::to_string(p1[2] / p1[3]) + "}"
        //     // + "     {" + std::to_string(p2[0] / p2[3]) + " " + std::to_string(p2[1] / p2[3]) + " " + std::to_string(p2[2] / p2[3]) + "}"
        //     + "     {" + std::to_string(p3[0] / p3[3]) + " " + std::to_string(p3[1] / p3[3]) + " " + std::to_string(p3[2] / p3[3]) + "}");
        if (xRel == 0 && yRel == 0) return;

        verticalAngle += xRel * sensitivity;
        if (verticalAngle >= 360.f) verticalAngle -= 360.f;
        if (verticalAngle <= -360.f) verticalAngle += 360.f;
        
        horizontalAngle += yRel * sensitivity;
        if (horizontalAngle >= 90.f) horizontalAngle = 89.f;
        if (horizontalAngle <= -90.f) horizontalAngle = -89.f;

        float camX = 0.f,
              camY = 0.f,
              camZ = 0.f;

        //  horizontal rotaion (rotate around horizont)
        float horRad = ZC_Vec::Radians(horizontalAngle);
        camY = camDistance * cos(horRad);
        camZ = camDistance * sin(horRad);

        //  vertical rotation (rotate around axis arthogonal to horizont)
        float vertRad = ZC_Vec::Radians(verticalAngle);
        camX = camY * cos(vertRad);
        camY = camY * sin(vertRad);
        
        camera.SetCamPos({ camX + position[0], camY + position[1], camZ + position[2] });

        // ZC_cout("angX = " + std::to_string(angleX) + "angY = " + std::to_string(angleY) + "x = " + std::to_string(camX) + "y = " + std::to_string(camY) + "z = " + std::to_string(camZ));

        CalculateDirections();
    }

    void CalculateDirections()
    {
        front = ZC_Vec::Normalize(camera.GetLookOn() - camera.GetCamPos());
        right = ZC_Vec::Cross(front, {0.f,0.f,1.f});    //  world up
        up = ZC_Vec::Cross(right, front);
    }
};

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
        auto iter = std::find(vertices.begin(), vertices.end(), vertex);
        if (iter == vertices.end()) vertices.emplace_back(Vertex{vertex, {vData}});
        else iter->vdList.emplace_back(vData);
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

    void CommonEdgeCenter(ZC_Vec3<float>& rCECS, ZC_Vec3<float>& rCEC, const ZC_Vec3<float>& point, bool& rIsEP1, bool& rIsEP2, NewSquare& rNS1, NewSquare& rNS2,
        void(NewSquare::*pSet1)(const ZC_Vec3<float>&), void(NewSquare::*pSet2)(const ZC_Vec3<float>&), unsigned short& rCommonEdges)
    {
        rCECS += rCEC;
        rIsEP1 = true;
        (rNS1.*pSet1)(point);
        rIsEP1 = true;
        (rNS2.*pSet2)(point);
        ++rCommonEdges;
    }
};


// struct ZCR_Figures
// {
//     static inline std::list<ZCR_Figure> figures;
// };



int ZC_main()
{

    ZC_upWindow window = ZC_Video::MakeWindow(true, width, height, "lolka");

    window->SetClearColor(0.4f, 0.4f, 0.4f);
    window->InitImGui();

    Player player({0.f,0.f,0.f}, 1000);
    ZC_ShProgs shProgs(false);
    typedef typename ZC_ShProgs::Name ShPName; 
    ShPName shPNames[] { ShPName::Color, ShPName::Point, ShPName::Line, ShPName::Stencil };
    shProgs.Load(shPNames, sizeof(shPNames) / sizeof(ShPName));

    ZC_Renderer::EnablePointSize();
    ZC_RS::SetStencilShaderProgData(ZC_ShProgs::Get(ZC_ShProgs::Name::Stencil));
    
    ZCR_Cube cube;

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

        bool a = true;
            bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glEnable(GL_DEPTH_TEST);

    auto image = ZC_Textures::LoadTexture2D("/home/dmitry/Загрузки/awesomeface.png");
    
    while (true)
    {
        if (!window->HandleEvents())
        {
           break;
        }            
        
        // ImGui::ShowDemoWindow(&a);

            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            
            static const float w = 300.f, h = 500.f;
            ImGui::SetWindowSize(ImVec2(w, h));
            ImGui::SetWindowPos({io.DisplaySize.x - w, 0.f});


            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            
            ImGui::Checkbox("Demo Window", &a);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::ImageButton("asf", (void*)image.id, ImVec2(50,50));
            if (ImGui::Button("Button", ImVec2(20,20)))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", io.DisplaySize.x, io.Framerate);
            ImGui::End();
            
        window->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ZC_Renderer::DrawAll();

        window->SwapBuffer();
    }
    
    return 0;
}
        

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