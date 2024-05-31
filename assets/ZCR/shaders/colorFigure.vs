#version 460 core



layout (location = 0) in vec3 inPosition;
// layout (location = 1) in vec3 inColor;
layout (location = 2) in vec4 inNormal;
layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};
uniform mat4 unModel;
uniform float unAlpha = 1.f;
uniform bool unUseLight;

out vec4 vColor;

void main()
{
    vec4 position = unModel * vec4(inPosition, 1.f);
    gl_Position = perspView * position;
    
    //  color calculation
    vec3 colorGrey = vec3(0.5f, 0.5f, 0.5f);

    if (unUseLight)     //  calculate light, otherwise uses alpha blending, don't use ligh
    {
        vec3 normal = normalize(mat3(unModel) * inNormal.xyz);   //  rotate and scale normals (wrong results for scaled object, but tolerable)
        vec3 dirPosToCam = normalize(camPos - position.xyz);
        float cos = dot(dirPosToCam, normal); //  range (-1,1)

        // float coef = max(0.f, cos);     //  avoid negative range (black color if angle less then 90 degree)
        float coef = (cos + 1.f) / 2.f;   //  from (-1,1) to (0,1)
        colorGrey *= coef;
    }
    vColor = vec4(colorGrey, unAlpha);
}