#pragma once

#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RSPersonalData.h>

enum ZCR_MatModelAxis
{
    ZCR_MMA_X,
    ZCR_MMA_Y,
    ZCR_MMA_Z
};

class ZCR_MatModel
{
public:
    struct Rotation
    {
        ZCR_MatModelAxis axis;
        float angle;

        void Rotate(ZC_Mat4<float>& _model)
        {
            if (angle != 0.f)
                _model.Rotate(angle, axis == ZCR_MMA_X ? ZC_Vec3<float>(1.f, 0.f, 0.f) : axis == ZCR_MMA_Y ? ZC_Vec3<float>(0.f, 1.f, 0.f) : ZC_Vec3<float>(0.f, 0.f, 1.f));
        }
    };

    ZC_Mat4<float> model { 1.f };

    ZCR_MatModel(ZC_RSPDStencilBorderData* _pStencilBorderData)
        : pStencilBorderData(_pStencilBorderData)
    {}

    float GetTranslate(ZCR_MatModelAxis axis)
    {
        switch (axis)
        {
        case ZCR_MMA_X: return translate[0];
        case ZCR_MMA_Y: return translate[1];
        case ZCR_MMA_Z: return translate[2];
        default: return 0.f;
        }
    }

    /*
    Sets new translate value for specific axis.
    
    Params:
    axis - axis.
    value - new value.
    calculateModel - need recalculate model (if need change more than one axis, true uses on last)
    */
    void SetTranslate(ZCR_MatModelAxis axis, float value, bool calculateModel)
    {
        switch (axis)
        {
        case ZCR_MMA_X: ChangeValue(translate[0], value, calculateModel); break;
        case ZCR_MMA_Y: ChangeValue(translate[1], value, calculateModel); break;
        case ZCR_MMA_Z: ChangeValue(translate[2], value, calculateModel); break;
        }
    }

    float GetRotateAngle(ZCR_MatModelAxis axis)
    {
        switch (axis)
        {
        case ZCR_MMA_X: return rotations[2].angle;
        case ZCR_MMA_Y: return rotations[1].angle;
        case ZCR_MMA_Z: return rotations[0].angle;
        default: return 0.f;
        }
    }

    /*
    Sets new rotate angle for specific axis.
    
    Params:
    axis - axis.
    value - new value.
    calculateModel - need recalculate model (if need change more than one axis, true uses on last)
    */
    void SetRotateAngle(ZCR_MatModelAxis axis, float angle, bool calculateModel)
    {
        switch (axis)
        {
        case ZCR_MMA_X: ChangeValue(rotations[2].angle, angle, calculateModel); break;
        case ZCR_MMA_Y: ChangeValue(rotations[1].angle, angle, calculateModel); break;
        case ZCR_MMA_Z: ChangeValue(rotations[0].angle, angle, calculateModel); break;
        }
    }

    float GetScale(ZCR_MatModelAxis axis)
    {
        switch (axis)
        {
        case ZCR_MMA_X: return scale[0];
        case ZCR_MMA_Y: return scale[1];
        case ZCR_MMA_Z: return scale[2];
        default: return 0.f;
        }
    }

    /*
    Sets new scale value for specific axis.
    
    Params:
    axis - axis.
    value - new value.
    calculateModel - need recalculate model (if need change more than one axis, true uses on last)
    */
    void SetScale(ZCR_MatModelAxis axis, float value, bool calculateModel)
    {
        auto lambChangeScale = [this, value, calculateModel](float& rCaurValue, float& rStencilBorderData)
        {
            ChangeValue(rCaurValue, value, calculateModel);
            float positiveValue = value < 0.f ? -value : value;
            rStencilBorderData = 1.f / positiveValue * (positiveValue + 0.05f);     //  to have same border after scale, recalculates the border value (0.05f -> border length)
        };

        switch (axis)
        {
        case ZCR_MMA_X: lambChangeScale(scale[0], pStencilBorderData->scaleX); break;
        case ZCR_MMA_Y: lambChangeScale(scale[1], pStencilBorderData->scaleY); break;
        case ZCR_MMA_Z: lambChangeScale(scale[2], pStencilBorderData->scaleZ); break;
        }
    }

private:
    ZC_RSPDStencilBorderData* pStencilBorderData;
    ZC_Vec3<float> translate;
    Rotation rotations[3] { { ZCR_MMA_Z, 0.f }, { ZCR_MMA_Y, 0.f }, { ZCR_MMA_X, 0.f } };   //  reverse order Z, Y, X
    ZC_Vec3<float> scale { 1.f, 1.f, 1.f };

    void CalculateModel()
    {   //  everything in back order
        model = ZC_Mat4<float>(1.f);
        model.Translate(translate);     //  final translate
        for (size_t i = 0; i < 3; ++i) rotations[i].Rotate(model);    //  rotations initialize in reverse order, so just make rotation Z, Y, X
        model.Scale(scale);
    }

    void ChangeValue(float& rCurValue, float newValue, bool calculateModel)
    {
        if (rCurValue == newValue) return;
        rCurValue = newValue;
        if (calculateModel) this->CalculateModel();
    };
};