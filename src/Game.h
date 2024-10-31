#pragma once

#include "Camera.h"
#include "Engine.h"
#include "Grid.h"
#include "Mines.h"
#include "Rail.h"
#include "Trains.h"

namespace Game
{
    const i32 LEVEL_COUNT = 1;

    struct Clock
    {
        float deltaTime;
        float timeScale = 1.0f;
    };

    struct Level
    {
        bool unlocked;

        Grid  ::Grid      grid;
        Mines ::State     mineState;
        Rail  ::RailState railState;
        Trains::State     trainState;
    };

    struct State
    {
        bool running;
        Clock clock;
        GameCamera::Camera camera;
        Vector2 mouseWorldPosition;
        
        i32 currency = 0;
        i32 currentLevel = 0;
        Level levels[LEVEL_COUNT];
    };
    extern State state;

    void Init();
    void Update();
    void Draw();

    inline Level& GetLevel(i32 level)
    {
        return state.levels[level];
    }
}