#include "Camera.h"

void GameCamera::Update(Camera& camera)
{
    float deltaTime = GetFrameTime();
    float zoomSpeed = 75.0f;

    if (IsKeyDown(KEY_Q) && camera.zoomTarget != MIN_ZOOM)
    {
        camera.zoomTarget -= ZOOM_INCREMENT * deltaTime * zoomSpeed;
        camera.target = Vector2Lerp(camera.target, GetWorldMousePosition(camera), deltaTime * 2.0f);
    }
    if (IsKeyDown(KEY_E) && camera.zoomTarget != MAX_ZOOM)
    {
        camera.zoomTarget += ZOOM_INCREMENT * deltaTime * zoomSpeed;
        camera.target = Vector2Lerp(camera.target, GetWorldMousePosition(camera), deltaTime *2.0f);
    }
    if (GetMouseWheelMove() < 0 && camera.zoomTarget != MIN_ZOOM)
    {
        camera.zoomTarget -= ZOOM_INCREMENT * deltaTime * zoomSpeed * 3;
        camera.target = Vector2Lerp(camera.target, GetWorldMousePosition(camera), deltaTime * 2.0f);
    }
    if (GetMouseWheelMove() > 0 && camera.zoomTarget != MAX_ZOOM)
    {
        camera.zoomTarget += ZOOM_INCREMENT * deltaTime * zoomSpeed * 3;
        camera.target = Vector2Lerp(camera.target, GetWorldMousePosition(camera), deltaTime * 2.0f);
    }

    if (camera.zoomTarget < MIN_ZOOM)
    {
        camera.zoomTarget = MIN_ZOOM;
    }
    if (camera.zoomTarget > MAX_ZOOM)
    {
        camera.zoomTarget = MAX_ZOOM;
    }

    camera.rlCamera.zoom = Lerp(camera.rlCamera.zoom, camera.zoomTarget, deltaTime * 5.0f);
    camera.rlCamera.target = Vector2Lerp(camera.rlCamera.target, camera.target, deltaTime * 5.0f);
}
