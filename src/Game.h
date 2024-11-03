#pragma once

#include "Camera.h"
#include "Engine.h"
#include "Grid.h"
#include "Mines.h"
#include "Rail.h"
#include "Trains.h"

namespace Game
{
    const i32 LEVEL_COUNT = 11;
    const i32 MAX_STACKS = 8;
    const i32 MAX_TRAINS = 8;

    struct Clock
    {
        float deltaTime = 0.0f;
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
        
        Color helpTextColor = PALETTE[BLACK_CARGO];
        std::string helpText;

        i32 trainsAvailable = 1;
        i32 mineCount    = 1;
        i32 stationCount = 1;
        i32 carCount     = 8;
        i32 railCount    = 8;
        
        bool trainDeployed = false;
        i32 trainCount   = 0;
        Trains::TrainEngine trains[MAX_TRAINS];

        std::vector<UnlockStack> unlockStacks;

        Vector2 UIPosition = { 0,0 };
        Rectangle bounds;
    };

    struct GameState
    {
        bool running;
        Clock clock;
        GameCamera::Camera camera;
        Vector2 mouseWorldPosition;
        
        Grid::Grid grid;
        i32 focusedLevel = 0;
        
        i32 currency = 0;
        Level levels[LEVEL_COUNT];

        bool level10StacksCreated = false;
        std::vector<Mines::Stack*> stacksToUnlockLevel10;
    };
    extern GameState state;

    void Init();

    void Update();
    void Draw();

    inline Level& GetLevel()
    {
        return state.levels[state.focusedLevel];
    }

    void CheckLevelStates();
    void CheckLevel(i32 level);
    void AddLevel(i32 level);
    void CreateLevel10Stacks();
}