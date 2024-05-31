// #version 460 core

// layout (points) in;
// layout (line_strip, max_vertices = 200) out;     // 200 * 2 => 200 total lines on both axis(100 on one axis), 2 line need to draw for one total line( |_ )

// in mat4 vPerspView[];

// out vec3 vColor;

// const float lineLength = 100.f;
// const float lineHalfLength = lineLength / 2.f;

// // void BuildMeshOnXY(vec4 position);

// float FillXYaxisX(float currentX)
// {
//     for (int i = 0; i < lineHalfLength / 2 - 1; ++i)
//     {
//         gl_Position = vPerspView[0] * vec4(currentX, -lineHalfLength, 0, 1.f);
//         EmitVertex();
//         gl_Position = vPerspView[0] * vec4(++currentX, -lineHalfLength, 0, 1.f);
//         EmitVertex();
//         gl_Position = vPerspView[0] * vec4(currentX, lineHalfLength, 0, 1.f);
//         EmitVertex();
//         gl_Position = vPerspView[0] * vec4(++currentX, lineHalfLength, 0, 1.f);
//         EmitVertex();
//     }
//     return currentX;
// }

// float FillXYaxisY(float currentY)
// {
//     for (int i = 0; i < lineHalfLength / 2 - 1; ++i)
//     {
//         gl_Position = vPerspView[0] * vec4(-lineHalfLength, currentY, 0, 1.f);
//         EmitVertex();
//         gl_Position = vPerspView[0] * vec4(-lineHalfLength, --currentY, 0, 1.f);
//         EmitVertex();
//         gl_Position = vPerspView[0] * vec4(lineHalfLength, currentY, 0, 1.f);
//         EmitVertex();
//         gl_Position = vPerspView[0] * vec4(lineHalfLength, --currentY, 0, 1.f);
//         EmitVertex();
//     }
//     return currentY;
// }

// void BuildMeshOnXY(vec4 position)   //  must started from vec3(-50, 50, 0)
// {
//     vColor = vec3(0.5f,0.5f,0.5f);

//     float currentX = -lineHalfLength;

//     gl_Position = vPerspView[0] * vec4(currentX, lineHalfLength, 0, 1.f);
//     EmitVertex();

//     currentX = FillXYaxisX(currentX);

//     gl_Position = vPerspView[0] * vec4(currentX, -lineHalfLength, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(++currentX, -lineHalfLength, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(currentX, lineHalfLength, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(++currentX, lineHalfLength, 0, 1.f); //  center red
//     vColor = vec3(1.f, 0.f, 0.f);
//     EmitVertex();

//     gl_Position = vPerspView[0] * vec4(currentX, -lineHalfLength, 0, 1.f); //  center still red
//     EmitVertex();
//     vColor = vec3(0.5f,0.5f,0.5f);  //  return grey
//     gl_Position = vPerspView[0] * vec4(++currentX, -lineHalfLength, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(currentX, lineHalfLength, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(++currentX, lineHalfLength, 0, 1.f);
//     EmitVertex();

//     FillXYaxisX(currentX);


//     float currentY = gl_Position.y;
//     currentY = FillXYaxisY(currentY);

//     gl_Position = vPerspView[0] * vec4(-lineHalfLength, currentY, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(-lineHalfLength, --currentY, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(lineHalfLength, currentY, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(lineHalfLength, --currentY, 0, 1.f);
//     vColor = vec3(0.f, 1.f, 0.f);
//     EmitVertex();

//     gl_Position = vPerspView[0] * vec4(-lineHalfLength, currentY, 0, 1.f);
//     EmitVertex();
//     vColor = vec3(0.5f,0.5f,0.5f);  //  return grey
//     gl_Position = vPerspView[0] * vec4(-lineHalfLength, --currentY, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(lineHalfLength, currentY, 0, 1.f);
//     EmitVertex();
//     gl_Position = vPerspView[0] * vec4(lineHalfLength, --currentY, 0, 1.f);
//     EmitVertex();

//     FillXYaxisY(currentY);
    
//     EndPrimitive();
// }

// void main()
// {
//     BuildMeshOnXY(gl_in[0].gl_Position);
//     // inPosition.z == 0 ? BuildMeshOnXY(gl_in[0].gl_Position);    //  lie on XY
//     // : inPosition.y == 0 ? 1     //  lie on ZX
//     // : 2;                        //  lie on ZY

// }