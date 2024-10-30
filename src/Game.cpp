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
        state.camera = GameCamera::Camera{};
        state.camera.target = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        state.camera.zoomTarget = 1.0f;
        state.running = true;

        state.camera.rlCamera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        state.camera.rlCamera.target = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        state.camera.rlCamera.rotation = 0.0f;
        state.camera.rlCamera.zoom = 1.0f;
    }

    void Update()
    {
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
    }

    void Draw()
    {
        DrawFPS(linePositionX, 20);
        //linePositionY += LINE_HEIGHT;

        std::string currencyText = "CURRENCY: " + std::to_string(state.currency);
        DrawText(currencyText.c_str(), linePositionX, 40, fontSize, PALETTE_DARK_GREEN);
        linePositionY += LINE_HEIGHT;
    }
}