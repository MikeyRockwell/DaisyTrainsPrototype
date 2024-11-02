#pragma once
#include "Engine.h"

namespace UI
{
    const i32 BUTTON_SIZE = 80;
    const Color normalColor   = PALETTE_LIGHT_GRAY;
    const Color hoveredColor  = PALETTE_PURPLE;
    const Color selectedColor = PALETTE_ORANGE;

    enum BuildType
    {
        RAIL,
        TRAIN,
        REMOVE_TRAIN,
        MINE,
        STATION,
        COUNT
    };

    struct Button
    {
        BuildType type;
        Textures::Sprite sprite;
        Rectangle rectangle;
        bool hovered;
        bool selected;
    };

    struct State
    {
        bool mouseOverUI = false;
        BuildType buildType;
        Button buttons[COUNT];
    };
    extern State state;

    void Init();
    void Update();
    void DrawWorldSpace();
    void DrawScreenSpace();
}