#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};
uniform mat4 unModel;

out vec4 vColor;

void main()
{
    vec4 worldPsition = unModel * vec4(inPosition, 1.f);
    vec3 dirPosToCam = normalize(camPos - vec3(worldPsition).xyz) * 0.02f;
    vec3 newPos = inPosition + dirPosToCam;
    gl_Position = perspView * unModel * vec4(newPos, 1.f);

    vColor = worldPsition.y == 0.f && worldPsition.z == 0 ? vec4(0.8f, 0.f, 0.f, 1.f)
        : worldPsition.x == 0.f && worldPsition.z == 0.f ? vec4(0.f, 0.8f, 0.f, 1.f)
        : worldPsition.x == 0.f || worldPsition.y == 0.f ? vec4(0.f, 0.6f, 0.8f, 1.f)
        : vec4(inColor, 1.f);
}