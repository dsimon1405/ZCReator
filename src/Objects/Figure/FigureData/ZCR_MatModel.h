#pragma once

#include <ZC/Tools/Math/ZC_Mat4.h>

#include <vector>

class ZCR_MatModel
{
public:
    enum Axis
    {
        A_X,
        A_Y,
        A_Z
    };

    struct Rotation
    {
        Axis axis;
        float angle;

        void Rotate(ZC_Mat4<float>& _model)
        {
            if (angle != 0.f)
                _model.Rotate(angle, axis == A_X ? ZC_Vec3<float>(1.f, 0.f, 0.f) : axis == A_Y ? ZC_Vec3<float>(0.f, 1.f, 0.f) : ZC_Vec3<float>(0.f, 0.f, 1.f));
        }
    };

    float GetTranslate(Axis axis)
    {
        switch (axis)
        {
        case A_X: return translate[0];
        case A_Y: return translate[1];
        case A_Z: return translate[2];
        }
    }

    void SetTranslate(Axis axis, float value, bool recalculateModel)
    {
        switch (axis)
        {
        case A_X: ChangeValue(translate[0], value, recalculateModel); break;
        case A_Y: ChangeValue(translate[1], value, recalculateModel); break;
        case A_Z: ChangeValue(translate[2], value, recalculateModel); break;
        }
    }

    float GetRotateAngle(Axis axis)
    {
        switch (axis)
        {
        case A_X: return rotations[2].angle;
        case A_Y: return rotations[1].angle;
        case A_Z: return rotations[0].angle;
        }
    }

    void SetRotateAngle(Axis axis, float angle, bool recalculateModel)
    {
        switch (axis)
        {
        case A_X: ChangeValue(rotations[2].angle, angle, recalculateModel); break;
        case A_Y: ChangeValue(rotations[1].angle, angle, recalculateModel); break;
        case A_Z: ChangeValue(rotations[0].angle, angle, recalculateModel); break;
        }
    }

    float GetScale(Axis axis)
    {
        switch (axis)
        {
        case A_X: return scale[0];
        case A_Y: return scale[1];
        case A_Z: return scale[2];
        }
    }

    void SetScale(Axis axis, float value, bool recalculateModel)
    {
        switch (axis)
        {
        case A_X: ChangeValue(scale[0], value, recalculateModel); break;
        case A_Y: ChangeValue(scale[1], value, recalculateModel); break;
        case A_Z: ChangeValue(scale[2], value, recalculateModel); break;
        }
    }

protected:
    ZC_Mat4<float> model { 1.f };

    ZCR_MatModel() = default;

private:
    ZC_Vec3<float> translate;
    Rotation rotations[3] { { A_Z, 0.f }, { A_Y, 0.f }, { A_X, 0.f } };   //  in reverse order Z, Y, X
    ZC_Vec3<float> scale { 1.f, 1.f, 1.f };

    void CalculateModel()
    {   //  everything in back order
        model = ZC_Mat4<float>(1.f);
        model.Translate(translate);     //  final translate
        for (size_t i = 0; i < 3; ++i) rotations->Rotate(model);    //  rotations initialize in reverse order, so just make rotation Z, Y, X
        model.Scale(scale);
    }

    void ChangeValue(float& rCurValue, float newValue, bool recalculateModel)
    {
        if (rCurValue == newValue) return;
        rCurValue = newValue;
        if (recalculateModel) this->CalculateModel();
    };
};