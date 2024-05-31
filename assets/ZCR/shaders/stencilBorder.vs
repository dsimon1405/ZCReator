#version 460 core

layout (location = 0) in vec3 inPosition;
layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};
uniform mat4 unModel;
uniform uint unColor;

out vec4 vColor;

void main()
{
    vec4 position = perspView * unModel * vec4(inPosition, 1.f);
    position.z = 0;     //  stencil border wrights to Z buffer, but must be uper every thing in to scene, so sets Z value  = 0
    gl_Position = position;

    float r = (unColor >> 20) / 255.f;
    float g = (unColor >> 10 & uint(1023)) / 255.f;
    float b = (unColor & uint(1023)) / 255.f;
    vColor = vec4(r, g, b, 1.f);
}