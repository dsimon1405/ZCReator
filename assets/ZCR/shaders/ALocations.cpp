// #version 460 core

// layout (location = 0) in vec2 inPosition;    on cpu 2 float
// layout (location = 0) in vec3 inPosition;    on cpu 3 float
// layout (location = 1) in vec3 inColor;       on cpu 3 uchar
// layout (location = 2) in vec4 inNormal;      on cpu 1 GL_INT_2_10_10_10_REV
// layout (location = 3) in vec2 inTexCoords;   on cpu 2 ushort

//  !!! pay attention about alignment in std140     https://ru.stackoverflow.com/questions/921705/%D0%92%D1%8B%D1%80%D0%B0%D0%B2%D0%BD%D0%B8%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5-std140-%D0%B4%D0%BB%D1%8F-unform-%D0%B1%D0%BB%D0%BE%D0%BA%D0%BE%D0%B2-%D0%B2-%D1%88%D0%B5%D0%B9%D0%B4%D0%B5%D1%80%D0%B0%D1%85-opengl-glsl
// layout (std140, binding = 0) uniform Camera
// {
//     mat4 ortho;
//     mat4 perspView;
//     vec3 camPos;
// };

// uniform mat4 unModel;
// uniform uint unColor;  stencil.vs
// uniform uint unPosition;  textWindow.vs

// uniform sampler2D texColor;