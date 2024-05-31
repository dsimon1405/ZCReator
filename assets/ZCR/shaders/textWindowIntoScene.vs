#version 460 core
layout (location = 0) in vec2 inPosition;   //  position into bottom left corner of the window (centralized by X axis -> ZC_Fonts::Font::CalcutateXYOriginBottomCenter(...))
layout (location = 1) in vec2 inTexCoords;
layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};

uniform vec3 unPositionScene;

out vec2 vTexCoords;

float ToZeroOneRange(float val)     //  [-1,1] -> [0,1]
{
    return (val + 1.0) / 2.0;
}

float ToMinusPlusOneRange(float val)    //  [0,1] -> [-1,1]
{
    return (val * 2.0) - 1.0;
}

void main()
{
    //  can't translate object's start position to scene position using [-1,1] range, cause go to [0,1] range

    vec4 posStart_mpo = ortho * vec4(inPosition, 0, 1);     //  [-1,1] mpo -> minus mpus one range
    vec2 posStart_zo = vec2(ToZeroOneRange(posStart_mpo.x), ToZeroOneRange(posStart_mpo.y));    //  [0,1] zo -> zero one range
    
    vec4 posScene_w = perspView * vec4(unPositionScene, 1);   //  position with w coefficient
    vec2 posScene_mpo = vec2(posScene_w.x / posScene_w.w,posScene_w.y / posScene_w.w);    //  [-1,1] mpo -> minus mpus one range
    vec2 posScene_zo = vec2(ToZeroOneRange(posScene_mpo.x), ToZeroOneRange(posScene_mpo.y));    //  [0,1] zo -> zero one range

    vec2 endPos_zo = posStart_zo + posScene_zo,     //  translates to scene position, [0,1]
        endPos_mpo = vec2(ToMinusPlusOneRange(endPos_zo.x), ToMinusPlusOneRange(endPos_zo.y)),  //  [-1,1]
        endPos_w = endPos_mpo * posScene_w.w;      //  returns to scene's position depth coefficient

    gl_Position = vec4(endPos_w, posScene_w.z, posScene_w.w);   //  returns to [-1,1]
    vTexCoords = inTexCoords;
}