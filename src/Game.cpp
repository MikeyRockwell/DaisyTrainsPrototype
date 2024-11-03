#include "Game.h"
#include "Audio.h"

namespace Game
{
    const i32 LINE_HEIGHT = 20;
    int linePositionY = 20;
    int linePositionX = 20;
    i32 fontSize = 20;
    GameState state;

    void Init()
    {
        // GRID
        state.grid = Grid::Init(0, 3, 4, CELL_SIZE);
        Grid::Grid& grid = state.grid;

        // CAMERA INIT
        state.camera = GameCamera::Camera{};
        state.camera.target = { (float)grid.width * CELL_SIZE / 2, (float)grid.height * CELL_SIZE / 2 };
        state.camera.zoomTarget = 1.0f;
        state.running = true;

        state.camera.rlCamera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        state.camera.rlCamera.target = state.camera.target;
        state.camera.rlCamera.rotation = 0.0f;
        state.camera.rlCamera.zoom = 1.0f;

       
    }

    void Update()
    {
        state.clock.deltaTime = GetFrameTime() * state.clock.timeScale;

        // TODO: Remove??
        if (IsKeyPressed(KEY_EQUAL))
        {
            state.clock.timeScale += 0.1f;
        }
        if (IsKeyPressed(KEY_MINUS))
        {
            state.clock.timeScale -= 0.1f;
        }

        Vector2 moveDirection = { 0, 0 };
        if (IsKeyDown(KEY_W))
        {
            moveDirection.y = -1;
        }
        if (IsKeyDown(KEY_S))
        {
            moveDirection.y = 1;
        }
        if (IsKeyDown(KEY_A))
        {
            moveDirection.x = -1;
        }
        if (IsKeyDown(KEY_D))
        {
            moveDirection.x = 1;
        }
        moveDirection = Vector2Normalize(moveDirection);
        moveDirection = Vector2Scale(moveDirection, state.camera.moveSpeed * GetFrameTime());
        state.camera.target = Vector2Add(state.camera.target, moveDirection);

        GameCamera::Update(state.camera);

        state.mouseWorldPosition = GameCamera::GetWorldMousePosition(state.camera);

        state.grid.Update();

#ifndef PLATFORM_WEB
        // LEVEL UNLOCKS
        if (IsKeyPressed(KEY_ONE))
        {
            AddLevel(1);
        }
        if (IsKeyPressed(KEY_TWO))
        {
            AddLevel(2);
        }
        if (IsKeyPressed(KEY_THREE))
        {
            AddLevel(3);
        }
        if (IsKeyPressed(KEY_FOUR))
        {
            AddLevel(4);
        }
        if (IsKeyPressed(KEY_FIVE))
        {
            AddLevel(5);
        }
        if (IsKeyPressed(KEY_SIX))
        {
            AddLevel(6);
        }
        if (IsKeyPressed(KEY_SEVEN))
        {
            AddLevel(7);
        }
        if (IsKeyPressed(KEY_EIGHT))
        {
            AddLevel(8);
        }
        if (IsKeyPressed(KEY_NINE))
        {
            AddLevel(9);
        }
        if (IsKeyPressed(KEY_ZERO))
        {
            AddLevel(10);
        }
        if (IsKeyPressed(KEY_TAB))
        {
            for (int i = 1; i < LEVEL_COUNT; i++)
            {
                AddLevel(i);
            }
        }

        if (IsKeyPressed(KEY_B))
        {
            Vector2Int mouseCoordinate = Grid::GetCoordinate(state.mouseWorldPosition);
            Grid::Cell* cell = state.grid.CoordinateToCell(mouseCoordinate);
            if (cell->initialized)
            {
                cell->hasObstacle = !cell->hasObstacle;
            }
        }
        if (IsKeyPressed(KEY_C))
        {
            Vector2Int mouseCoordinate = Grid::GetCoordinate(state.mouseWorldPosition);
            Grid::Cell* cell = state.grid.CoordinateToCell(mouseCoordinate);
            if (cell->initialized)
            {
                if (cell->railType == Rail::CROSSING)
                {
                    cell->railType = -1;
                    cell->connectionPoints.reset();
                    cell->connectionPositions[0] = { -1,-1 };
                    cell->connectionPositions[1] = { -1,-1 };
                    cell->hasCrossing = false;
                    // Remove the rail from the rail state
                    Rail::railState.coordinateToRailMap.erase(cell->coordinate);
                }
                else
                {
                    Rail::CreatePermanentRail(cell, Rail::CROSSING, false);
                }
            }
        }
#endif

        for (int i = 0; i < LEVEL_COUNT; i++)
        {
            Level& level = state.levels[i];
            if (CheckCollisionPointRec(state.mouseWorldPosition, level.bounds))
            {
                state.focusedLevel = i;
            }
        }
    }

    void Draw()
    {
        for (int i = 0; i < LEVEL_COUNT; i++)
        {
            Level& level = state.levels[i];
            if (level.unlocked)
            {
                Color borderColor = state.focusedLevel == i ? PALETTE_ORANGE : PALETTE_LIGHT_GRAY;
                DrawRectangleLinesEx(level.bounds, 4.0f / state.camera.rlCamera.zoom, borderColor);
            }
        }
    }

    void CheckLevelStates()
    {
        i32 levelsUnlocked = 0;
        for (int i = 0; i < LEVEL_COUNT; i++)
        {
            Level& level = state.levels[i];
            if (level.unlocked)
            {
                levelsUnlocked++;
                CheckLevel(i);
            }
        }
        if (levelsUnlocked == 8)
        {
            if (!state.level10StacksCreated)
            {
                CreateLevel10Stacks();
                state.level10StacksCreated = true;
            }
        }

        if (state.level10StacksCreated && !state.levels[10].unlocked)
        {
            i32 stacksFull = 0;
            for (int i = 0; i < state.stacksToUnlockLevel10.size(); i++)
            {
                Mines::Stack* stack = state.stacksToUnlockLevel10[i];
                if (stack->count == stack->capacity)
                {
                    stacksFull++;
                }
            }
            if (stacksFull == state.stacksToUnlockLevel10.size())
            {
                AddLevel(10);
            }
        }
    }

    void CheckLevel(i32 level)
    {
        Level& currentLevel = state.levels[level];
        for (int i = 0; i < currentLevel.unlockStacks.size(); i++)
        {
            UnlockStack& unlockStack = currentLevel.unlockStacks[i];
            if (unlockStack.unlocked)
            {
                continue;
            }
            Mines::Stack* stack = &Mines::state.stacks[unlockStack.stackCoordinate];
            if (stack->count == stack->capacity)
            {
                unlockStack.unlocked = true;
                if (!state.levels[unlockStack.levelToUnlock].unlocked)
                {
                    AddLevel(unlockStack.levelToUnlock);
                    PlaySound(Audio::resources.unlock_level.sound);
                }
            }
        }
    }

    void AddLevel(i32 level)
    {
        if (level == 0)
        {
            Level& level = state.levels[0];
            level.bounds = { 0, 0, 3 * CELL_SIZE, 4 * CELL_SIZE};

            Mines::Stack* pinkStackFull  = Mines::CreateStack({ 1, -1 }, PINK_CARGO, 500, 500);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 1,  4 }, PINK_CARGO, 50, 0);
            
            level.unlockStacks.push_back({ false, 1, { 1, 4 } });
            level.number = 0;
            level.unlocked = true;
            level.helpText = "Welcome to\nSky High Stacks!\n\nPlease transport the\npink cargo to the\nempty stack";
            level.mineCount = 1;
            level.stationCount = 1;
            level.trainsAvailable = 1;
            level.carCount = 8;
            level.railCount = 10;
            level.helpTextColor = PALETTE[PINK_CARGO];
            level.UIPosition = { -250 , 0 };
        }

        if (level == 1)
        {
            Level& level = state.levels[1];
            Grid::AddToGrid(state.grid, { -1, 5 }, 5, 5);
            level.bounds = { -1 * CELL_SIZE, 5 * CELL_SIZE, 5 * CELL_SIZE, 5 * CELL_SIZE };

            // Stacks
            Mines::CreateStack({ 2, 8 }, YELLOW_CARGO, 2000, 2000); // PICKUP STACK
            Mines::Stack* pinkStack   = Mines::CreateStack({ 4, 6 }, PINK_CARGO, 50, 0);
            Mines::Stack* orangeStack = Mines::CreateStack({ 0, 10 }, YELLOW_CARGO, 100, 0);
             
            // Resources
            level.mineCount = 2;
            level.stationCount = 2;
            level.railCount = 16;
            level.trainsAvailable = 1;
            
            // Obstacles
            state.grid.cells[{ -1, 5 }].hasObstacle = true;
            state.grid.cells[{ -1, 6 }].hasObstacle = true;
            state.grid.cells[{ -1, 7 }].hasObstacle = true;

            // Unlock stacks
            level.unlocked = true;
            level.unlockStacks.push_back({ false, 2, { 0, 10 } });
            level.unlockStacks.push_back({ false, 3, { 4, 6  } });
        }

        if (level == 2) // LEVEL 3 - 3 GRIDS DOWN
        {
            Level& level = state.levels[2];
            Grid::AddToGrid(state.grid, { -1, 11 }, 5, 5);
            level.bounds = { -1 * CELL_SIZE, 11 * CELL_SIZE, 5 * CELL_SIZE, 5 * CELL_SIZE };

            i32 pinkCargo = 500;
            Mines::Stack* pinkStackFull = Mines::CreateStack ({ 1, 13 }, PINK_CARGO, pinkCargo, pinkCargo);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 4, 12 }, PINK_CARGO, 100, 0);
            Mines::Stack* orangeStackEmpty = Mines::CreateStack({ 0, 16 }, YELLOW_CARGO, 100, 0);

            // Resources
            level.mineCount = 2;
            level.stationCount = 2;
            level.railCount = 21;
            level.trainsAvailable = 1;

            // Obstacles
            state.grid.cells[{ 2, 14 }].hasObstacle = true;
            state.grid.cells[{ 3, 14 }].hasObstacle = true;
            state.grid.cells[{ 2, 15 }].hasObstacle = true;
            state.grid.cells[{ 3, 15 }].hasObstacle = true;

            level.unlocked = true;
            level.unlockStacks.push_back({ false, 4, pinkStackEmpty->coordinate });
            level.unlockStacks.push_back({ false, 5, orangeStackEmpty->coordinate });
        }

        if (level == 3) // LEVEL 4 - 2 DOWN, 1 RIGHT
        {
            Level& level = state.levels[3];
            Grid::AddToGrid(state.grid, { 5, 5 }, 7, 5);
            level.bounds = { 5 * CELL_SIZE, 5 * CELL_SIZE, 7 * CELL_SIZE, 5 * CELL_SIZE };

            i32 purpleCargo = 500;
            Mines::Stack* blueStackFull  = Mines::CreateStack({ 7, 7 },  PURPLE_CARGO, purpleCargo, purpleCargo);
            Mines::Stack* blueStackEmpty = Mines::CreateStack({ 6, 4 },  PURPLE_CARGO, 100, 0);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 9, 4 },  PINK_CARGO, 150, 0);

            // Resources
            level.mineCount = 4;
            level.stationCount = 3;
            level.railCount = 35;
            level.trainsAvailable = 1;

            // Obstacles
            state.grid.cells[{ 8, 7 }].hasObstacle = true;

            level.unlocked = true;
            level.unlockStacks.push_back({ false, 9, blueStackEmpty->coordinate });
        }

        if (level == 4) // 2 DOWN, 2 RIGHT
        {
            Level& level = state.levels[4];
            Grid::AddToGrid(state.grid, { 5, 11 }, 7, 5);
            level.bounds = { 5 * CELL_SIZE, 11 * CELL_SIZE, 7 * CELL_SIZE, 5 * CELL_SIZE };
            level.unlocked = true;

            i32 pinkCargo = 500;
            Mines::Stack* pinkStackFull  = Mines::CreateStack({ 9, 16 }, PINK_CARGO, pinkCargo, pinkCargo);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 9, 10 }, PINK_CARGO, 150, 0);

            // Resources
            level.mineCount = 3;
            level.stationCount = 2;
            level.railCount = 35;
            level.trainsAvailable = 1;

            // Obstacles
            state.grid.cells[{ 11, 11 }].hasObstacle = true;
            state.grid.cells[{ 10, 13 }].hasObstacle = true;
            state.grid.cells[{ 11, 14 }].hasObstacle = true;
            state.grid.cells[{ 6, 12 }].hasObstacle = true;
            state.grid.cells[{ 6, 14 }].hasObstacle = true;
            state.grid.cells[{ 7, 13 }].hasObstacle = true;

            // Crossroads
            Rail::CreatePermanentRail(&state.grid.cells[{8, 14}], Rail::CROSSING, true);

            // TODO: Add unlock stack
        }

        if (level == 5)
        {
            Level& level = state.levels[5];
            Grid::AddToGrid(state.grid, { -1, 17 }, 13, 6);
            level.bounds = { -1 * CELL_SIZE, 17 * CELL_SIZE, 13 * CELL_SIZE, 6 * CELL_SIZE };

            i32 orangeCargo = 1000;
            i32 purpleCargo = 500;
            Mines::Stack* greenStackFull   = Mines::CreateStack({ 4, 20 }, ORANGE_CARGO, orangeCargo, orangeCargo);
            Mines::Stack* greenStackEmpty  = Mines::CreateStack({ 6, 16 }, ORANGE_CARGO, 150, 0);
            Mines::Stack* blueStackFull    = Mines::CreateStack({ 8, 20 }, PURPLE_CARGO, purpleCargo, purpleCargo);
            Mines::Stack* blueStackEmpty   = Mines::CreateStack({ 12, 18 }, PURPLE_CARGO, 150, 0);
            Mines::Stack* orangeStackEmpty = Mines::CreateStack({ 12, 21 }, YELLOW_CARGO, 150, 0);

            Mines::Stack* greenStackEmptyLevel4 = Mines::CreateStack({ 6,10 }, ORANGE_CARGO, 100, 0);

            // Resources
            level.mineCount = 3;
            level.stationCount = 3;
            level.railCount = 85;
            level.trainsAvailable = 2;

            // Obstacles
            state.grid.cells[{ 1, 19 }].hasObstacle = true;
            state.grid.cells[{ 2, 19 }].hasObstacle = true;
            state.grid.cells[{ 1, 20 }].hasObstacle = true;
            state.grid.cells[{ 2, 20 }].hasObstacle = true;
            state.grid.cells[{ 1, 21 }].hasObstacle = true;
            state.grid.cells[{ 2, 21 }].hasObstacle = true;
            state.grid.cells[{ 8, 21 }].hasObstacle = true;
            state.grid.cells[{ 9, 17 }].hasObstacle = true;
            state.grid.cells[{ 5, 19 }].hasObstacle = true;

            // Unlocks
            level.unlocked = true;
            level.unlockStacks.push_back({ false, 7, orangeStackEmpty->coordinate });
        }

        if (level == 6)
        {
            Level& level = state.levels[6];
            Grid::AddToGrid(state.grid, { 13, 11 }, 8, 5);
            level.bounds = { 13 * CELL_SIZE, 11 * CELL_SIZE, 8 * CELL_SIZE, 5 * CELL_SIZE };

            i32 orangeCargo = 1000;
            Mines::Stack* orangeStackFull  = Mines::CreateStack({ 14, 13 }, ORANGE_CARGO, orangeCargo, orangeCargo);
            Mines::Stack* orangeStackEmpty = Mines::CreateStack({ 14, 10 }, ORANGE_CARGO, 150, 0);
            Mines::Stack* blueStackEmpty2  = Mines::CreateStack({ 21, 14 }, PURPLE_CARGO, 150, 0);
            
            // Resources
            level.mineCount = 3;
            level.stationCount = 2;
            level.railCount = 85;
            level.trainsAvailable = 2;

            // Obstacles
            state.grid.cells[{ 14, 12 }].hasObstacle = true;
            state.grid.cells[{ 13, 15 }].hasObstacle = true;

            // Crossing
            Rail::CreatePermanentRail(&state.grid.cells[{15, 14}], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{17, 13}], Rail::RailType::CROSSING, false);

            // Rail
            Rail::CreatePermanentRail(&state.grid.cells[{15, 15}], Rail::RailType::TOP_TO_RIGHT, false);
            Rail::CreatePermanentRail(&state.grid.cells[{15, 11}], Rail::RailType::BOTTOM_TO_LEFT, true);

            // TODO: Add unlock stack
            level.unlocked = true;
            //level.unlockStacks.push_back({ false, 8, orangeStackEmpty->coordinate });
        }

        if (level == 7)
        {
            Level& level = state.levels[7];
            Grid::AddToGrid(state.grid, { 13, 17 }, 8, 6);
            level.bounds = { 13 * CELL_SIZE, 17 * CELL_SIZE, 8 * CELL_SIZE, 6 * CELL_SIZE };

            Mines::CreateStack({ 21, 20 }, YELLOW_CARGO, 200, 0);
            Mines::Stack* unlockStack = Mines::CreateStack({ 16, 16 }, PURPLE_CARGO  , 150, 0);
            
            // Resources
            level.mineCount = 2;
            level.stationCount = 2;
            level.railCount = 85;
            level.trainsAvailable = 2;
            
            // Obstacles
            state.grid.cells[{ 14, 18 }].hasObstacle = true;
            state.grid.cells[{ 16, 20 }].hasObstacle = true;
            state.grid.cells[{ 17, 20 }].hasObstacle = true;
            state.grid.cells[{ 16, 21 }].hasObstacle = true;
            state.grid.cells[{ 17, 21 }].hasObstacle = true;
            state.grid.cells[{ 18, 17 }].hasObstacle = true;
            state.grid.cells[{ 19, 17 }].hasObstacle = true;
            state.grid.cells[{ 20, 17 }].hasObstacle = true;
            state.grid.cells[{ 18, 18 }].hasObstacle = true;
            state.grid.cells[{ 19, 18 }].hasObstacle = true;
            state.grid.cells[{ 20, 18 }].hasObstacle = true;

            // Crossing
            Rail::CreatePermanentRail(&state.grid.cells[{15, 19}], Rail::RailType::CROSSING, false);
            // Unlocks
            level.unlocked = true;
            level.unlockStacks.push_back({ false, 6, unlockStack->coordinate });
        }

        if (level == 8)
        {
            Level& level = state.levels[8];
            Grid::AddToGrid(state.grid, { 13, 5 }, 8, 5);
            level.bounds = { 13 * CELL_SIZE, 5 * CELL_SIZE, 8 * CELL_SIZE, 5 * CELL_SIZE };

            Mines::Stack* greenStackEmpty  = Mines::CreateStack({ 21, 6 }, ORANGE_CARGO, 150, 0);
            Mines::Stack* greenStackEmpty2 = Mines::CreateStack({ 12, 8 }, ORANGE_CARGO, 150, 0);
            Mines::Stack* blueStackEmpty   = Mines::CreateStack({ 17, 10 }, PURPLE_CARGO, 150, 0);
            
            // Resources
            level.mineCount = 4;
            level.stationCount = 2;
            level.railCount = 85;
            level.trainsAvailable = 2;
            
            // Obstacles
            state.grid.cells[{ 14, 6 }].hasObstacle = true;
            state.grid.cells[{ 16, 7 }].hasObstacle = true;
            state.grid.cells[{ 17, 7 }].hasObstacle = true;
            
            // Crossing
            Rail::CreatePermanentRail(&state.grid.cells[{18, 6}], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{18, 8}], Rail::RailType::CROSSING, false);

            // Unlocks
            level.unlocked = true;
        }

        if (level == 9)
        {
            Level& level = state.levels[9];
            Vector2Int start = { 5, -2 };
            float width = 16;
            float height = 6;
            Grid::AddToGrid(state.grid, { start.x, start.y }, width, height);
            level.bounds = { (float)start.x * CELL_SIZE, (float)start.y * CELL_SIZE, width * CELL_SIZE, height * CELL_SIZE };
            level.unlocked = true;

            Mines::Stack* pinkStackFull  = Mines::CreateStack({  7, -1 }, PINK_CARGO  , 500, 500);
            Mines::Stack* blueStackFull  = Mines::CreateStack({ 12,  0 }, PURPLE_CARGO, 1000, 1000);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 21,  0 }, PINK_CARGO  , 250, 0);
            Mines::Stack* blueStackEmpty = Mines::CreateStack({ 17,  4 }, PURPLE_CARGO, 250, 0);

            // Resources
            level.mineCount = 4;
            level.stationCount = 2;
            level.railCount = 85;
            level.trainsAvailable = 4;

            // Obstacles
            state.grid.cells[{   6, 0 }].hasObstacle = true;
            state.grid.cells[{  11, 1 }].hasObstacle = true;
            state.grid.cells[{  14, 0 }].hasObstacle = true;
            state.grid.cells[{  14, 2 }].hasObstacle = true;
            state.grid.cells[{  15, 2 }].hasObstacle = true;
            state.grid.cells[{  17, 0 }].hasObstacle = true;
            state.grid.cells[{  18, 0 }].hasObstacle = true;
            state.grid.cells[{  17, -1 }].hasObstacle = true;
            state.grid.cells[{  18, -1 }].hasObstacle = true;
            
            // Crossing
            Rail::CreatePermanentRail(&state.grid.cells[{ 7, 1 }],  Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 7, 2 }],  Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 8, 2 }],  Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 10, 0 }], Rail::RailType::CROSSING, false); // ASK FUN DIMENSION
            Rail::CreatePermanentRail(&state.grid.cells[{ 12, 2 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 16, 1 }], Rail::RailType::CROSSING, false);

            // Rails...
            Rail::CreatePermanentRail(&state.grid.cells[{ 6, 1 }], Rail::RailType::HORIZONTAL, true);

            // Unlocks
            level.unlockStacks.push_back({ false, 8, blueStackEmpty->coordinate });
        }

        if (level == 10)
        {
            Level& level = state.levels[10];
            Vector2Int start = { 22, -2 };
            float width = 8;
            float height = 25;
            Grid::AddToGrid(state.grid, { start.x, start.y }, width, height);
            level.bounds = { (float)start.x * CELL_SIZE, (float)start.y * CELL_SIZE, width * CELL_SIZE, height * CELL_SIZE };
            level.unlocked = true;

            Mines::Stack* pinkStackEmpty   = Mines::CreateStack({ 30, 20 }, PINK_CARGO,   2000, 0);
            Mines::Stack* purpleStackEmpty = Mines::CreateStack({ 30, 6  }, PURPLE_CARGO, 2000, 0);
            Mines::Stack* yellowStackEmpty = Mines::CreateStack({ 30, 0  }, YELLOW_CARGO, 2000, 0);
            Mines::Stack* orangeStackEmpty = Mines::CreateStack({ 30, 14 }, ORANGE_CARGO, 2000, 0);

            // Resources
            level.mineCount = 4;
            level.stationCount = 4;
            level.railCount = 185;
            level.trainsAvailable = 8;
            level.carCount = 16;

            // Crossing
            Rail::CreatePermanentRail(&state.grid.cells[{ 24, 0 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 25, 0 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 24, 2 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 25, 2 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 28, 2 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 25, 8 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 25, 10 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 27, 10 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 28, 10 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 25, 11 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 27, 11 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 25, 14 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 23, 17 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 24, 17 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 28, 18 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 26, 19 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 26, 20 }], Rail::RailType::CROSSING, false);
            Rail::CreatePermanentRail(&state.grid.cells[{ 27, 20 }], Rail::RailType::CROSSING, false);

            // Obstacles
            state.grid.cells[{ 23, -1 }].hasObstacle = true;
            state.grid.cells[{ 26, -1 }].hasObstacle = true;
            state.grid.cells[{ 27, -1 }].hasObstacle = true;
            state.grid.cells[{ 28, -1 }].hasObstacle = true;
            state.grid.cells[{ 27,  1 }].hasObstacle = true;
            state.grid.cells[{ 23,  3 }].hasObstacle = true;
            state.grid.cells[{ 23,  4 }].hasObstacle = true;
            state.grid.cells[{ 25,  4 }].hasObstacle = true;
            state.grid.cells[{ 26,  4 }].hasObstacle = true;
            state.grid.cells[{ 25,  5 }].hasObstacle = true;
            state.grid.cells[{ 26,  5 }].hasObstacle = true;
            state.grid.cells[{ 28,  5 }].hasObstacle = true;
            state.grid.cells[{ 23,  6 }].hasObstacle = true;
            state.grid.cells[{ 28,  6 }].hasObstacle = true;
            state.grid.cells[{ 26,  7 }].hasObstacle = true;
            state.grid.cells[{ 27,  7 }].hasObstacle = true;
            state.grid.cells[{ 23,  8 }].hasObstacle = true;
            state.grid.cells[{ 23,  9 }].hasObstacle = true;
            state.grid.cells[{ 24,  9 }].hasObstacle = true;
            state.grid.cells[{ 26,  9 }].hasObstacle = true;
            state.grid.cells[{ 24,  12 }].hasObstacle = true;
            state.grid.cells[{ 23,  13 }].hasObstacle = true;
            state.grid.cells[{ 26,  13 }].hasObstacle = true;
            state.grid.cells[{ 28,  13 }].hasObstacle = true;
            state.grid.cells[{ 23,  14 }].hasObstacle = true;
            state.grid.cells[{ 28,  14 }].hasObstacle = true;
            state.grid.cells[{ 23,  15 }].hasObstacle = true;
            state.grid.cells[{ 28,  15 }].hasObstacle = true;
            state.grid.cells[{ 26,  16 }].hasObstacle = true;
            state.grid.cells[{ 27,  17 }].hasObstacle = true;
            state.grid.cells[{ 25,  18 }].hasObstacle = true;
            state.grid.cells[{ 23,  21 }].hasObstacle = true;
            state.grid.cells[{ 24,  21 }].hasObstacle = true;
            state.grid.cells[{ 28,  21 }].hasObstacle = true;

        }
    }

    void CreateLevel10Stacks()
    {
        state.stacksToUnlockLevel10.push_back(&Mines::state.stacks[{ 21, 0  }]);
        state.stacksToUnlockLevel10.push_back(&Mines::state.stacks[{ 21, 14 }]);
        state.stacksToUnlockLevel10.push_back(&Mines::state.stacks[{ 21, 6  }]);
        state.stacksToUnlockLevel10.push_back(&Mines::state.stacks[{ 21, 20 }]);
    }
}