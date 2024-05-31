#version 460 core
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoords;
layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};
uniform vec2 unPositionWindow;

out vec2 vTexCoords;

void main()
{
    gl_Position = ortho * vec4(inPosition.x + unPositionWindow.x, inPosition.y + unPositionWindow.y, 0, 1);
    vTexCoords = inTexCoords;
}