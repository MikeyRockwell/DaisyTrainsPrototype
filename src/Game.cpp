#include "Game.h"

namespace Game
{
    const i32 LINE_HEIGHT = 20;
    int linePositionY = 20;
    int linePositionX = 20;
    i32 fontSize = 20;
    State state;

    void Init()
    {
        state.levels[0].grid = Grid::Init(5, 5, CELL_SIZE);
        Grid::Grid& grid = state.levels[0].grid;

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

        state.levels[state.currentLevel].grid.Update();
    }

    void Draw()
    {
        DrawFPS(linePositionX, 20);

        std::string currencyText = "CURRENCY: " + std::to_string(state.currency);
        DrawText(currencyText.c_str(), linePositionX, 40, fontSize, PALETTE_GREEN);
        linePositionY += LINE_HEIGHT;
    }
}