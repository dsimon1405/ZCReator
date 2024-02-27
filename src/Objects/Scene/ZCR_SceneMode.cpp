#include "ZCR_SceneMode.h"

#include "ZCR_SMModel.h"

ZC_uptr<ZCR_SceneMode> ZCR_SceneMode::GetuptrOnMode(Mode mode)
{
    switch (mode)
    {
    case Mode::Model: return ZC_uptrMakeFromChild<ZCR_SceneMode, ZCR_SMModel>();
    }
}