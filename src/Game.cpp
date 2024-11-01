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
    }

    void Draw()
    {
        DrawFPS(linePositionX, 20);

        std::string currencyText = "CURRENCY: " + std::to_string(state.currency);
        DrawText(currencyText.c_str(), linePositionX, 40, fontSize, PALETTE_GREEN);
        linePositionY += LINE_HEIGHT;
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
                AddLevel(unlockStack.levelToUnlock);
            }
        }
    }

    void AddLevel(i32 level)
    {
        if (level == 0)
        {
            Level& level = state.levels[0];
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
            level.trackCount = 8;
            level.helpTextColor = PALETTE[PINK_CARGO];
            level.UIPosition = { -250 , 0 };
        }

        if (level == 1)
        {
            Grid::AddToGrid(state.grid, { -1, 5 }, 5, 5);
            i32 orangeCargo = 100;
            Mines::CreateStack({ 2, 8 }, ORANGE_CARGO, orangeCargo, orangeCargo); // PICKUP STACK
            Mines::Stack* pinkStack   = Mines::CreateStack({ 4, 6 }, PINK_CARGO, 50, 0);
            Mines::Stack* orangeStack = Mines::CreateStack({ 0, 10 }, ORANGE_CARGO, orangeCargo, 0);
            Mines::state.minesAvailable    += 200;
            Mines::state.stationsAvailable += 200;
            Rail::railState.railAvailable  += 220;
            Trains::state.trainsAvailable  += 100;
            
            Level& level = state.levels[1];
            level.unlocked = true;
            level.unlockStacks.push_back({ false, 2, { 0, 10 } });
            level.unlockStacks.push_back({ false, 3, { 4, 6  } });
        }

        if (level == 2) // LEVEL 3 - 3 GRIDS DOWN
        {
            Grid::AddToGrid(state.grid, { -1, 11 }, 5, 5);
            i32 pinkCargo = 100;
            Mines::Stack* pinkStackFull = Mines::CreateStack ({ 1, 13 }, PINK_CARGO, pinkCargo, pinkCargo);
            Mines::Stack* pinkStackEmpty = Mines::CreateStack({ 4, 12 }, PINK_CARGO, pinkCargo, 0);

            Level& level = state.levels[2];
            level.unlocked = true;
            level.unlockStacks.push_back({ false, 4, { 4, 12 } });
        }

        if (level == 3) // LEVEL 4 - 2 DOWN, 1 RIGHT
        {
            Grid::AddToGrid(state.grid, { 5, 5 }, 6, 5);
            i32 blueCargo = 200;
            Mines::Stack* blueStackFull  = Mines::CreateStack({ 7, 7 }, BLUE_CARGO, blueCargo, blueCargo);
            Mines::Stack* blueStackEmpty = Mines::CreateStack({ 7, 4 }, BLUE_CARGO, blueCargo, 0);

            Level& level = state.levels[3];
            level.unlocked = true;
            // TODO: Add unlock stack
        }


    }
}