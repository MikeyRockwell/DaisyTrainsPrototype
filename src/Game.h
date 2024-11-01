#pragma once

#include "Camera.h"
#include "Engine.h"
#include "Grid.h"
#include "Mines.h"
#include "Rail.h"
#include "Trains.h"

namespace Game
{
    const i32 LEVEL_COUNT = 5;
    const i32 MAX_STACKS = 8;

    struct Clock
    {
        float deltaTime;
        float timeScale = 1.0f;
    };

    struct UnlockStack
    {
        bool unlocked = false;
        i32 levelToUnlock;
        Vector2Int stackCoordinate;
    };

    // Resources for the current level
    struct Level
    {
        i32 number;
        bool unlocked;
        //bool complete;
        
        Color helpTextColor = PALETTE[BLACK_CARGO];
        std::string helpText;

        i32 mineCount    = 1;
        i32 stationCount = 1;
        i32 trainCount   = 1;
        i32 carCount     = 8;
        i32 trackCount   = 8;

        std::vector<UnlockStack> unlockStacks;

        Vector2 UIPosition = { 0,0 };
    };

    struct GameState
    {
        bool running;
        Clock clock;
        GameCamera::Camera camera;
        Vector2 mouseWorldPosition;
        
        Grid  ::Grid grid;
        
        i32 currency = 0;
        i32 currentLevel = 0;
        Level levels[LEVEL_COUNT];
    };
    extern GameState state;

    void Init();

    void Update();
    void Draw();

    inline Level& GetLevel(i32 levelNumber)
    {
        return state.levels[levelNumber];
    }

    void CheckLevelStates();
    void CheckLevel(i32 level);
    void AddLevel(i32 level);
    
}