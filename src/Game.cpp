#include "Game.h"

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

        if (IsKeyPressed(KEY_TWO))
        {
            AddLevel(1);
        }
        if (IsKeyPressed(KEY_THREE))
        {
            AddLevel(2);
        }
        if (IsKeyPressed(KEY_FOUR))
        {
            AddLevel(3);
        }
        if (IsKeyPressed(KEY_FIVE))
        {
            AddLevel(4);
        }
        if (IsKeyPressed(KEY_SIX))
        {
            AddLevel(5);
        }
        if (IsKeyPressed(KEY_SEVEN))
        {
            AddLevel(6);
        }
        if (IsKeyPressed(KEY_EIGHT))
        {
            AddLevel(7);
        }
        if (IsKeyPressed(KEY_NINE))
        {
            AddLevel(8);
        }
        if (IsKeyPressed(KEY_ZERO))
        {
            AddLevel(9);
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
        for (int i = 0; i < LEVEL_COUNT; i++)
        {
            Level& level = state.levels[i];
            if (level.unlocked)
            {
                CheckLevel(i);
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

            Mines::Stack* pinkStackFull  = Mines::CreateStack({ 1, -1 }, PINK_CARGO, 50, 50);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 1,  4 }, PINK_CARGO, 50, 0);
            
            level.unlockStacks.push_back({ false, 1, { 1, 4 } });
            level.number = 0;
            level.unlocked = true;
            level.helpText = "Welcome to\nSky High Stacks!\n\nPlease transport the\npink cargo to the\nempty stack";
            level.mineCount = 1;
            level.stationCount = 1;
            level.trainCount = 1;
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
            i32 orangeCargo = 100;
            Mines::CreateStack({ 2, 8 }, YELLOW_CARGO, orangeCargo, orangeCargo); // PICKUP STACK
            Mines::Stack* pinkStack   = Mines::CreateStack({ 4, 6 }, PINK_CARGO, 50, 0);
            Mines::Stack* orangeStack = Mines::CreateStack({ 0, 10 }, YELLOW_CARGO, orangeCargo, 0);
             
            // Resources
            level.mineCount = 2;
            level.stationCount = 2;
            level.railCount = 16;
            level.trainCount = 1;
            
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

            i32 pinkCargo = 100;
            Mines::Stack* pinkStackFull = Mines::CreateStack ({ 1, 13 }, PINK_CARGO, pinkCargo, pinkCargo);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 4, 12 }, PINK_CARGO, pinkCargo, 0);
            Mines::Stack* orangeStackEmpty = Mines::CreateStack({ 0, 16 }, YELLOW_CARGO, 100, 0);

            // Resources
            level.mineCount = 2;
            level.stationCount = 2;
            level.railCount = 21;
            level.trainCount = 1;

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
            Grid::AddToGrid(state.grid, { 5, 5 }, 6, 5);
            level.bounds = { 5 * CELL_SIZE, 5 * CELL_SIZE, 6 * CELL_SIZE, 5 * CELL_SIZE };

            i32 blueCargo = 200;
            Mines::Stack* blueStackFull  = Mines::CreateStack({ 7, 7 },  PURPLE_CARGO, blueCargo, blueCargo);
            Mines::Stack* blueStackEmpty = Mines::CreateStack({ 6, 4 },  PURPLE_CARGO, blueCargo, 0);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 9, 4 }, PINK_CARGO, 250, 0);

            // Resources
            level.mineCount = 4;
            level.stationCount = 3;
            level.railCount = 35;
            level.trainCount = 1;

            // Obstacles
            state.grid.cells[{ 8, 7 }].hasObstacle = true;

            level.unlocked = true;
            level.unlockStacks.push_back({ false, 9, pinkStackEmpty->coordinate });
        }

        if (level == 4) // 2 DOWN, 2 RIGHT
        {
            Level& level = state.levels[4];
            Grid::AddToGrid(state.grid, { 5, 11 }, 6, 5);
            level.bounds = { 5 * CELL_SIZE, 11 * CELL_SIZE, 6 * CELL_SIZE, 5 * CELL_SIZE };

            i32 pinkCargo = 100;
            Mines::Stack* pinkStackFull  = Mines::CreateStack({ 9, 16 }, PINK_CARGO, pinkCargo, pinkCargo);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 9, 10 }, PINK_CARGO, pinkCargo, 0);

            // Resources
            level.mineCount = 3;
            level.stationCount = 2;
            level.railCount = 35;
            level.trainCount = 1;

            // Obstacles
            state.grid.cells[{ 9, 12 }].hasObstacle = true;
            state.grid.cells[{ 9, 13 }].hasObstacle = true;
            state.grid.cells[{ 9, 14 }].hasObstacle = true;
            state.grid.cells[{ 6, 13 }].hasObstacle = true;
            state.grid.cells[{ 6, 14 }].hasObstacle = true;
            state.grid.cells[{ 7, 14 }].hasObstacle = true;

            level.unlocked = true;
            // TODO: Add unlock stack
        }

        if (level == 5)
        {
            Level& level = state.levels[5];
            Grid::AddToGrid(state.grid, { -1, 17 }, 13, 7);
            level.bounds = { -1 * CELL_SIZE, 17 * CELL_SIZE, 13 * CELL_SIZE, 7 * CELL_SIZE };

            i32 greenCargo = 200;
            i32 blueCargo  = 200;
            Mines::Stack* greenStackFull   = Mines::CreateStack({ 4, 20 }, ORANGE_CARGO, greenCargo, greenCargo);
            Mines::Stack* greenStackEmpty  = Mines::CreateStack({ 6, 16 }, ORANGE_CARGO, greenCargo, 0);
            Mines::Stack* blueStackFull    = Mines::CreateStack({ 8, 20 }, PURPLE_CARGO, blueCargo, blueCargo);
            Mines::Stack* blueStackEmpty   = Mines::CreateStack({ 12, 18 }, PURPLE_CARGO, blueCargo, 0);
            Mines::Stack* orangeStackEmpty = Mines::CreateStack({ 12, 21 }, YELLOW_CARGO, 100, 0);

            Mines::Stack* greenStackEmptyLevel4 = Mines::CreateStack({ 6,10 }, ORANGE_CARGO, 100, 0);

            // Resources
            level.mineCount = 3;
            level.stationCount = 3;
            level.railCount = 45;
            level.trainCount = 1;

            // Obstacles
            state.grid.cells[{ 1, 19 }].hasObstacle = true;
            state.grid.cells[{ 2, 19 }].hasObstacle = true;
            state.grid.cells[{ 1, 20 }].hasObstacle = true;
            state.grid.cells[{ 2, 20 }].hasObstacle = true;
            state.grid.cells[{ 1, 21 }].hasObstacle = true;
            state.grid.cells[{ 2, 21 }].hasObstacle = true;
            state.grid.cells[{ 8, 21 }].hasObstacle = true;
            state.grid.cells[{ 9, 17 }].hasObstacle = true;

            // Unlocks
            level.unlocked = true;
            level.unlockStacks.push_back({ false, 7, orangeStackEmpty->coordinate });
        }

        if (level == 6)
        {
            Level& level = state.levels[6];
            Grid::AddToGrid(state.grid, { 12, 11 }, 7, 5);
            level.bounds = { 12 * CELL_SIZE, 11 * CELL_SIZE, 7 * CELL_SIZE, 5 * CELL_SIZE };

            i32 greenCargo = 200;
            Mines::Stack* greenStackFull  = Mines::CreateStack({ 13, 13 }, ORANGE_CARGO, greenCargo, greenCargo);
            Mines::Stack* greenStackEmpty = Mines::CreateStack({ 13, 10 }, ORANGE_CARGO, greenCargo, 0);
            Mines::Stack* blueStackEmpty  = Mines::CreateStack({ 16, 10 }, PURPLE_CARGO, 100, 0);
            Mines::Stack* blueStackEmpty2 = Mines::CreateStack({ 19, 13 }, PURPLE_CARGO, 100, 0);
            
            // Resources
            level.mineCount = 3;
            level.stationCount = 2;
            level.railCount = 35;
            level.trainCount = 1;

            // Obstacles
            state.grid.cells[{ 13, 12 }].hasObstacle = true;
            state.grid.cells[{ 17, 13 }].hasObstacle = true;
            state.grid.cells[{ 16, 12 }].hasObstacle = true;
            state.grid.cells[{ 16, 14 }].hasObstacle = true;

            // Crossing
            Rail::CreateRailSegment(&state.grid.cells[{14, 14}], Rail::RailType::CROSSING);

            // TODO: Add unlock stack
            level.unlocked = true;
            level.unlockStacks.push_back({ false, 8, greenStackEmpty->coordinate });
        }

        if (level == 7)
        {
            Level& level = state.levels[7];
            Grid::AddToGrid(state.grid, { 13, 17 }, 6, 6);
            level.bounds = { 13 * CELL_SIZE, 17 * CELL_SIZE, 6 * CELL_SIZE, 6 * CELL_SIZE };

            Mines::Stack* orangeStackEmpty = Mines::CreateStack({ 19, 20 }, YELLOW_CARGO, 100, 0);
            Mines::Stack* blueStackEmpty   = Mines::CreateStack({ 16, 16 }, PURPLE_CARGO  , 100, 0);
            
            // Resources
            level.mineCount = 2;
            level.stationCount = 2;
            level.railCount = 35;
            level.trainCount = 1;
            
            // Obstacles
            state.grid.cells[{ 14, 18 }].hasObstacle = true;
            state.grid.cells[{ 16, 20 }].hasObstacle = true;
            state.grid.cells[{ 16, 21 }].hasObstacle = true;
            state.grid.cells[{ 17, 20 }].hasObstacle = true;
            state.grid.cells[{ 17, 21 }].hasObstacle = true;

            // Crossing
            Rail::CreateRailSegment(&state.grid.cells[{15, 19}], Rail::RailType::CROSSING);
            // Unlocks
            level.unlocked = true;
        }

        if (level == 8)
        {
            Level& level = state.levels[8];
            Grid::AddToGrid(state.grid, { 12, 5 }, 7, 5);
            level.bounds = { 12 * CELL_SIZE, 5 * CELL_SIZE, 7 * CELL_SIZE, 5 * CELL_SIZE };

            Mines::Stack* pinkStackEmpty   = Mines::CreateStack({ 13, 4 }, PINK_CARGO  , 100, 0);
            //Mines::Stack* blueStackEmpty = Mines::CreateStack({ 17, 4 }, PURPLE_CARGO, 100, 0);
            Mines::Stack* greenStackEmpty  = Mines::CreateStack({ 19, 6 }, ORANGE_CARGO, 100, 0);
            Mines::Stack* greenStackEmpty2 = Mines::CreateStack({ 11, 8 }, ORANGE_CARGO, 100, 0);
            
            // Resources
            level.mineCount = 4;
            level.stationCount = 2;
            level.railCount = 16;
            level.trainCount = 1;
            
            // Obstacles
            state.grid.cells[{ 13, 6 }].hasObstacle = true;
            state.grid.cells[{ 15, 7 }].hasObstacle = true;
            state.grid.cells[{ 16, 7 }].hasObstacle = true;
            
            // Crossing
            Rail::CreateRailSegment(&state.grid.cells[{17, 6}], Rail::RailType::CROSSING);
            Rail::CreateRailSegment(&state.grid.cells[{17, 8}], Rail::RailType::CROSSING);

            // Unlocks
            level.unlocked = true;
        }

        if (level == 9)
        {
            Level& level = state.levels[9];
            Grid::AddToGrid(state.grid, { 4, -2 }, 15, 6);
            level.bounds = { 4 * CELL_SIZE, -2 * CELL_SIZE, 15 * CELL_SIZE, 6 * CELL_SIZE };
            level.unlocked = true;

            Mines::Stack* pinkStackFull  = Mines::CreateStack({ 7 , 0 }, PINK_CARGO  , 1000, 0);
            Mines::Stack* blueStackFull  = Mines::CreateStack({ 12, 0 }, PURPLE_CARGO, 1000, 0);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 19, 1 }, PINK_CARGO  , 100, 0);
            Mines::Stack* blueStackEmpty = Mines::CreateStack({ 17, 4 }, PURPLE_CARGO  , 100, 0);

            // Resources
            level.mineCount = 4;
            level.stationCount = 2;
            level.railCount = 55;
            level.trainCount = 1;

            // Obstacles
            state.grid.cells[{  4, 0 }].hasObstacle = true;
            state.grid.cells[{  4, 1 }].hasObstacle = true;
            state.grid.cells[{  4, 2 }].hasObstacle = true;
            state.grid.cells[{  9, 0 }].hasObstacle = true;
            state.grid.cells[{ 10, 0 }].hasObstacle = true;
            state.grid.cells[{ 14, 0 }].hasObstacle = true;
            
            // Crossing
            Rail::CreateRailSegment(&state.grid.cells[{ 11, 2 }], Rail::RailType::CROSSING);
            Rail::CreateRailSegment(&state.grid.cells[{ 12, 2 }], Rail::RailType::CROSSING);
            Rail::CreateRailSegment(&state.grid.cells[{ 15, 1 }], Rail::RailType::CROSSING);
            Rail::CreateRailSegment(&state.grid.cells[{ 15, 2 }], Rail::RailType::CROSSING);
            Rail::CreateRailSegment(&state.grid.cells[{ 16, 2 }], Rail::RailType::CROSSING);
        }
    }
}