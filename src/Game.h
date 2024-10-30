#pragma once

#include "Engine.h"
#include "Camera.h"

namespace Game
{
    struct State
    {
        bool running;

        i32 currency = 0;
        GameCamera::Camera camera;
        Vector2 mouseWorldPosition;
    };
    extern State state;

    void Init();
    void Update();
    void Draw();
}